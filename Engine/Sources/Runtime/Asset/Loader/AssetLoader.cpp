#include <Core.h>
#include "AssetLoader.h"

namespace SE
{
	AAssetLoader::AAssetLoader() : SAddressable()
	{
		this->AssetDirectory = "";
	}

	AAssetLoader::AAssetLoader(const std::string& name, const std::string& assetDirectory) : SAddressable()
	{
		this->Initialize(name, assetDirectory);
	}

	AAssetLoader::AAssetLoader(const AAssetLoader & other) : SAddressable(other)
	{
		this->AssetDirectory = other.AssetDirectory;
		this->AssetList = other.AssetList;
	}

	AAssetLoader::~AAssetLoader()
	{

	}

	void AAssetLoader::Initialize(const std::string & name, const std::string & assetDirectory)
	{
		this->SetName(name);
		this->AssetDirectory = assetDirectory;

		std::vector<std::string> RawFilePathList;
		IterateDirectory(this->AssetDirectory, RawFilePathList);

		std::map<std::string, bool> LoadedChecker;
		for (const auto& rawFilePath : RawFilePathList)
		{
			LoadedChecker[rawFilePath] = false;
		}
		for (auto& [uuid, asset] : this->AssetList)
		{
			LoadedChecker[asset->GetRawFilePath()] = true;
		}
		for (auto& [_rawFilePath, isLoaded] : LoadedChecker)
		{
			if (!isLoaded)
			{
				std::shared_ptr<AAsset> Asset = std::make_shared<AAsset>();
				Asset->Import(_rawFilePath);

				std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);
				this->AssetList[Asset->GetUUID()] = Asset;
			}
		}

		this->AssetDirectoryObserver.Start("Engine/Assets",
			[this](DWORD action, const std::wstring& fileName)
			{
				static std::filesystem::path LastRenamedOldName = "";

				auto FilePath = std::filesystem::path(fileName);

				switch (action)
				{
					case FILE_ACTION_ADDED:
					{
						// TO DO: Show notification to ask if it should be imported.

						break;
					}

					case FILE_ACTION_REMOVED:
					{
						// TO DO: Show warning of unexpected file moving for asset.

						break;
					}

					case FILE_ACTION_MODIFIED:
					{
						std::string Category = AAsset::ValidateCategory(FilePath.extension().string());
						if (Category != "Unknown")
						{
							auto Data = AAsset::ImportData(FilePath.string(), Category);

							std::string FileName = FilePath.stem().string();

							std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);
							for (auto& [uuid, asset] : this->AssetList)
							{
								if (asset->GetName() == FileName)
								{
									asset->ResetData(Data);
									break;
								}
							}
						}

						break;
					}

					case FILE_ACTION_RENAMED_OLD_NAME:
					{
						if (std::filesystem::directory_entry(FilePath).is_directory())
						{
							LastRenamedOldName = FilePath;
						}

						break;
					}

					case FILE_ACTION_RENAMED_NEW_NAME:
					{
						auto DirectoryEntry = std::filesystem::directory_entry(FilePath);

						std::vector<std::filesystem::path> ChildRawFilePathList;

						std::function<void(std::filesystem::directory_entry)> IterateRenamedDirectory = 
							[&](std::filesystem::directory_entry directoryEntry) -> void
							{
								for (auto childEntry : std::filesystem::recursive_directory_iterator(directoryEntry))
								{
									if (childEntry.is_directory())
									{
										IterateRenamedDirectory(childEntry);
									}
									else if (childEntry.is_regular_file())
									{
										std::filesystem::path FilePath = childEntry.path();
										if (AAsset::ValidateCategory(FilePath.extension().string()) != "Unknown")
										{
											ChildRawFilePathList.push_back(FilePath);
										}
									}
								}
							};

						if (DirectoryEntry.is_directory())
						{
							IterateRenamedDirectory(DirectoryEntry);

							for (auto childRawFilePath : ChildRawFilePathList)
							{
								std::string FileName = childRawFilePath.stem().string();
								if (this->FindAsset(FileName))
								{
									std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);
									for (auto& [uuid, asset] : this->AssetList)
									{
										if (asset->GetName() == FileName)
										{
											asset->ResetPackage(childRawFilePath.parent_path());
											break;
										}
									}
								}
							}
						}

						break;
					}
				}
			});

		this->Activate();
	}

	void AAssetLoader::Update()
	{

	}

	bool AAssetLoader::FindAsset(const std::string& name) const noexcept
	{
		return false;
	}

	bool AAssetLoader::FindAsset(const SUUID& uuid) const noexcept
	{
		return false;
	}

	std::shared_ptr<AAsset> AAssetLoader::GetAsset(const std::string& name)
	{
		std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);

		for (auto& [uuid, asset] : this->AssetList)
		{
			if (asset->GetName() == name)
			{
				return asset;
			}
		}

		SMessageHandler::Instance->SetFatal("Asset",
			std::format("No registered asset named '{}' found in loader!", name));
		return null;
	}

	std::shared_ptr<AAsset> AAssetLoader::GetAsset(const SUUID& uuid)
	{
		std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);

		if (this->AssetList.count(uuid) <= 0)
		{
			SMessageHandler::Instance->SetFatal("Asset",
				std::format("No registered asset with id : '{}' found in loader!", (const std::string&)uuid));
		}

		return this->AssetList[uuid];
	}

	void AAssetLoader::IterateDirectory(std::filesystem::path directory, std::vector<std::string>& rawFilePathList)
	{
		for (const auto& directoryEntry : std::filesystem::recursive_directory_iterator(directory))
		{
			const auto& Path = directoryEntry.path();

			if (directoryEntry.is_regular_file())
			{
				this->LoadInAsset(Path, rawFilePathList);
			}
			else if (directoryEntry.is_directory())
			{
				IterateDirectory(Path, rawFilePathList);
			}
		}
	}

	void AAssetLoader::LoadInAsset(std::filesystem::path filePath, std::vector<std::string>& rawFilePathList)
	{
		std::shared_ptr<AAsset> Asset;

		if (filePath.extension() == ".sasset")
		{
			Asset = std::make_shared<AAsset>(filePath.string());

			std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);
			this->AssetList[Asset->GetUUID()] = Asset;
		}
		else if (AAsset::ValidateCategory(filePath.extension().string()) != "Unknown")
		{
			rawFilePathList.push_back(filePath.string());
		}
		else
		{
			// Do nothing, because the file is not a supported asset category.
		}
	}
}
