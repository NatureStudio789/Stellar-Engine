#ifndef _SE_ASSETLOADER_H_
#define _SE_ASSETLOADER_H_
#include "../../Core/Addressable/Addressable.h"
#include "../../Core/Registry/Registry.h"
#include "../../Function/DirectoryObserver/DirectoryObserver.h"

#include "../Asset.h"

namespace SE
{
	class AAssetLoader : public SAddressable
	{
	public:
		AAssetLoader();
		AAssetLoader(const std::string& assetDirectory);
		AAssetLoader(const AAssetLoader& other);
		~AAssetLoader();

		// #Multithreaded Function
		void Initialize(const std::string& assetDirectory);

		// #Multithreaded Function
		void Update();

		bool FindAsset(std::filesystem::path serializedOrRawFilePath) const noexcept;
		bool FindAsset(const SUUID& uuid) const noexcept;

		std::shared_ptr<AAsset> GetAsset(std::filesystem::path serializedOrRawFilePath);
		std::shared_ptr<AAsset> GetAsset(const SUUID& uuid);

	private:
		void RegisterAsset(std::shared_ptr<AAsset> asset);

		void IterateDirectory(std::filesystem::path directory, 
			std::vector<std::string>& rawFilePathList, std::vector<std::string>& serializedFilePathList);
		void LoadInAsset(std::filesystem::path filePath);

		std::filesystem::path AssetDirectory;

		mutable std::shared_mutex AssetMutex;
		std::map<SUUID, std::shared_ptr<AAsset>> AssetList;

		FDirectoryObserver AssetDirectoryObserver;
	};

	STELLAR_MAKE_DEFAULT_REGISTRY(AAssetLoader, AssetLoaderRegistry);
}

#endif