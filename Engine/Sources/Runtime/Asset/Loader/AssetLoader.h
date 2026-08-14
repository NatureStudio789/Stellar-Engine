#ifndef _SE_ASSETLOADER_H_
#define _SE_ASSETLOADER_H_
#include "../../Core/Addressable/Addressable.h"
#include "../../Function/DirectoryObserver/DirectoryObserver.h"

#include "../Asset.h"

namespace SE
{
	class AAssetLoader : public SAddressable
	{
	public:
		AAssetLoader();
		AAssetLoader(const std::string& name, const std::string& assetDirectory);
		AAssetLoader(const AAssetLoader& other);
		~AAssetLoader();

		// #Multithreaded Function
		void Initialize(const std::string& name, const std::string& assetDirectory);

		// #Multithreaded Function
		void Update();

		bool FindAsset(const std::string& name) const noexcept;
		bool FindAsset(const SUUID& uuid) const noexcept;

		std::shared_ptr<AAsset> GetAsset(const std::string& name);
		std::shared_ptr<AAsset> GetAsset(const SUUID& uuid);

	private:
		void IterateDirectory(std::filesystem::path directory, std::vector<std::string>& rawFilePathList);
		void LoadInAsset(std::filesystem::path filePath, std::vector<std::string>& rawFilePathList);

		std::filesystem::path AssetDirectory;

		mutable std::shared_mutex AssetMutex;
		std::map<SUUID, std::shared_ptr<AAsset>> AssetList;

		FDirectoryObserver AssetDirectoryObserver;
	};
}

#endif