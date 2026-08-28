#ifndef _SE_ASSET_H_
#define _SE_ASSET_H_
#include "../Core/Addressable/Addressable.h"
#include "../Core/Assetizable/Assetizable.h"

namespace SE
{
	/* AAsset:
	Name could NOT be used as an ID to address AAsset
	because, in different directories, of different categories, 
	different assets might get the same name from files in the same name.*/
	class AAsset : public SAddressable
	{
	public:
		AAsset(const std::string& belongingLoaderName);
		AAsset(const std::string& serializedFilePath, const std::string& belongingLoaderName);
		AAsset(const AAsset& other);
		~AAsset();

		// Initialize asset with the serialized file.
		void Load(const std::string& serializedFilePath);

		// Initialize asset by importing data from raw file.
		void Import(const std::string& rawFilePath);

		void AddBindingParent(SAssetizable* parent);

		const std::string& GetBelongingLoaderName() const noexcept;

		const std::string& GetCategory() const noexcept;
		const std::string& GetFilePackage() const noexcept;

		std::string GetRawFilePath() const noexcept;

		std::string GetSerializedFilePath() const noexcept;
		bool GetSerialized() const noexcept;

		template<typename DataType>
		DataType GetData()
		{
			return std::any_cast<DataType>(this->Data);
		}

	private:
		// #For hot reloading by Asset Loader.
		void ResetData(const std::any& data);
		void ResetPackage(const std::filesystem::path& package);

		static std::string ValidateCategory(std::string fileExtension);
		static std::any ImportData(std::string filePath, std::string category, std::string belongingLoaderName);

		// Set by that exact Asset Loader.
		std::string BelongingLoaderName;

		std::string Category;

		// The directory of the asset file.
		std::string FilePackage;

		// Path of the raw file which holds the source data.
		std::filesystem::path RawFilePath;
		std::string RawFileExtension;

		// Path of the file which has been serialized by Engine.
		std::filesystem::path SerializedFilePath;
		bool IsSerialized = false;

		std::any Data;

		std::vector<SAssetizable*> BindingParent;

		friend class AAssetLoader;
		friend class FAssetSerializer;

	public:
		// Asset Categories Enum :
		static const std::string Texture;
		static const std::string StaticMesh;
	};
}

#endif