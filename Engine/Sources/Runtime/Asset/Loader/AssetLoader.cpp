#include <Core.h>
#include "AssetLoader.h"

namespace SE
{
	AAssetLoader::AAssetLoader() : SAddressable()
	{
		this->AssetDirectory = "";

		this->Activate();
	}

	AAssetLoader::AAssetLoader(const std::string& assetDirectory) : SAddressable()
	{
		this->Initialize(assetDirectory);
	}

	AAssetLoader::AAssetLoader(const AAssetLoader & other) : SAddressable(other)
	{
		this->AssetDirectory = other.AssetDirectory;
		this->AssetList = other.AssetList;
	}

	AAssetLoader::~AAssetLoader()
	{

	}

	void AAssetLoader::Initialize(const std::string & assetDirectory)
	{
		this->AssetDirectory = assetDirectory;

		std::vector<std::string> RawFilePathList;
		std::vector<std::string> SerializedFilePathList;
		IterateDirectory(this->AssetDirectory, RawFilePathList, SerializedFilePathList);

		// Sort paths in categories to solve the dependencies between different kinds of assets.
		std::map<int, std::vector<std::string>> FilePathToLoadInCategories;

		for (auto serializedFilePath : SerializedFilePathList)
		{
			auto SerializedFilePath = std::filesystem::path(serializedFilePath);
			std::filesystem::path SerializedNoExtension = SerializedFilePath.parent_path() / SerializedFilePath.stem();

			for (auto iterator = RawFilePathList.begin(); iterator != RawFilePathList.end(); ++iterator)
			{
				auto RawFilePath = std::filesystem::path(*iterator);
				std::filesystem::path RawNoExtension = RawFilePath.parent_path() / RawFilePath.stem();

				if (SerializedNoExtension == RawNoExtension)
				{
					std::string Category = AAsset::ValidateCategory(RawFilePath.extension().string());
					int CategoryIndex = -1;
					if (Category == AAsset::Texture)
					{
						CategoryIndex = 0;
					}
					else if (Category == AAsset::StaticMesh)
					{
						CategoryIndex = 1;
					}

					FilePathToLoadInCategories[CategoryIndex].push_back(SerializedFilePath.string());

					RawFilePathList.erase(iterator);
					break;
				}
			}
		}

		for (auto RestRawFilePath : RawFilePathList)
		{
			std::filesystem::path RawFilePath = { RestRawFilePath };
			std::string Category = AAsset::ValidateCategory(RawFilePath.extension().string());

			int CategoryIndex = -1;
			if (Category == AAsset::Texture)
			{
				CategoryIndex = 0;
			}
			else if (Category == AAsset::StaticMesh)
			{
				CategoryIndex = 1;
			}

			FilePathToLoadInCategories[CategoryIndex].push_back(RestRawFilePath);
		}

		for (auto [index, filePathList] : FilePathToLoadInCategories)
		{
			for (auto assetFilePath : filePathList)
			{
				std::cout << "Loading in asset : " << assetFilePath << "\n";

				this->LoadInAsset(assetFilePath);
			}
		}

		this->AssetDirectoryObserver.Start("Engine/Assets",
			[this](DWORD action, const std::wstring& fileName)
			{
				static std::filesystem::path LastRenamedOldName = "";

				auto FilePath = this->AssetDirectory / std::filesystem::path(fileName);

				switch (action)
				{
					case FILE_ACTION_ADDED:
					{
						std::cout << "Asset File Added : " << FilePath << "\n";

						// TO DO: Show notification to ask if it should be imported.

						break;
					}

					case FILE_ACTION_REMOVED:
					{
						std::cout << "Asset File Removed : " << FilePath << "\n";

						// TO DO: Show warning of unexpected file moving for asset.

						break;
					}

					case FILE_ACTION_MODIFIED:
					{
						std::cout << "Asset File Modified : " << FilePath << "\n";

						std::string Category = AAsset::ValidateCategory(FilePath.extension().string());
						if (Category != "Unknown")
						{
							auto Data = AAsset::ImportData(FilePath.string(), Category, this->GetName());

							this->GetAsset(FilePath)->ResetData(Data);
						}

						break;
					}

					case FILE_ACTION_RENAMED_OLD_NAME:
					{
						if (std::filesystem::directory_entry(FilePath).is_directory())
						{
							LastRenamedOldName = this->AssetDirectory / FilePath;
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
								if (this->FindAsset(childRawFilePath))
								{
									this->GetAsset(childRawFilePath)->ResetPackage(childRawFilePath.parent_path());
								}
							}
						}

						break;
					}
				}
			});

	}

	void AAssetLoader::Update()
	{
		
	}

	bool AAssetLoader::FindAsset(std::filesystem::path serializedOrRawFilePath) const noexcept
	{
		if(serializedOrRawFilePath.extension() == ".sasset")
		{
			std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);

			for (auto [uuid, asset] : this->AssetList)
			{
				auto NormalizedAssetSerializePath = std::filesystem::canonical(asset->SerializedFilePath);
				auto NormalizedWantedSerializePath = std::filesystem::canonical(serializedOrRawFilePath);
				if (NormalizedAssetSerializePath == NormalizedWantedSerializePath)
				{
					return true;
				}
			}
		}
		else if (AAsset::ValidateCategory(serializedOrRawFilePath.extension().string()) != "Unknown")
		{
			std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);

			for (auto [uuid, asset] : this->AssetList)
			{
				auto NormalizedAssetRawPath = std::filesystem::canonical(asset->RawFilePath);
				auto NormalizedWantedRawPath = std::filesystem::canonical(serializedOrRawFilePath);
				if (NormalizedAssetRawPath == NormalizedWantedRawPath)
				{
					return true;
				}
			}
		}

		return false;
	}

	bool AAssetLoader::FindAsset(const SUUID& uuid) const noexcept
	{
		std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);
		return this->AssetList.count(uuid) > 0;
	}

	std::shared_ptr<AAsset> AAssetLoader::GetAsset(std::filesystem::path serializedOrRawFilePath)
	{
		if (serializedOrRawFilePath.extension() == ".sasset")
		{
			std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);

			for (auto [uuid, asset] : this->AssetList)
			{
				auto NormalizedAssetSerializePath = std::filesystem::canonical(asset->SerializedFilePath);
				auto NormalizedWantedSerializePath = std::filesystem::canonical(serializedOrRawFilePath);
				if (NormalizedAssetSerializePath == NormalizedWantedSerializePath)
				{
					return asset;
				}
			}
		}
		else if (AAsset::ValidateCategory(serializedOrRawFilePath.extension().string()) != "Unknown")
		{
			std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);

			for (auto [uuid, asset] : this->AssetList)
			{
				auto NormalizedAssetRawPath = std::filesystem::canonical(asset->RawFilePath);
				auto NormalizedWantedRawPath = std::filesystem::canonical(serializedOrRawFilePath);
				if (NormalizedAssetRawPath == NormalizedWantedRawPath)
				{
					return asset;
				}
			}
		}
		else
		{
			SMessageHandler::Instance->SetFatal("Asset",
				std::format("No registered asset with serialized file path : '{}' found in registry!", serializedOrRawFilePath.string()));
		}

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

	void AAssetLoader::RegisterAsset(std::shared_ptr<AAsset> asset)
	{
		std::unique_lock<std::shared_mutex> AssetLocker(this->AssetMutex);
		this->AssetList[asset->GetUUID()] = asset;
	}

	void AAssetLoader::IterateDirectory(std::filesystem::path directory, 
		std::vector<std::string>& rawFilePathList, std::vector<std::string>& serializedFilePathList)
	{
		for (const auto& directoryEntry : std::filesystem::recursive_directory_iterator(directory))
		{
			const auto& Path = directoryEntry.path();

			if (directoryEntry.is_regular_file())
			{
				if (Path.extension() == ".sasset")
				{
					serializedFilePathList.push_back(Path.string());
				}
				else if (AAsset::ValidateCategory(Path.extension().string()) != "Unknown")
				{
					rawFilePathList.push_back(Path.string());
				}
				else
				{
					// Do nothing, because the file is not a supported asset category.
				}
			}
			//else if (directoryEntry.is_directory())
			//{
			//	IterateDirectory(Path, rawFilePathList, serializedFilePathList);
			//}
		}
	}

	void AAssetLoader::LoadInAsset(std::filesystem::path filePath)
	{
		std::shared_ptr<AAsset> Asset;

		if (filePath.extension() == ".sasset")
		{
			Asset = std::make_shared<AAsset>(filePath.string(), this->GetName());
			Asset->BelongingLoaderName = this->GetName();

			this->RegisterAsset(Asset);
		}
		else if (AAsset::ValidateCategory(filePath.extension().string()) != "Unknown")
		{
			Asset = std::make_shared<AAsset>(this->GetName());
			Asset->Import(filePath.string());

			this->RegisterAsset(Asset);
		}
		else
		{
			SMessageHandler::Instance->SetFatal("Asset", 
				std::format("File : '{}' with '{}' extension\n"
					"	Unexpected file type for asset!", filePath.string(), filePath.extension().string()));
		}
	}
}
