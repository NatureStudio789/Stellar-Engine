#ifndef _SE_ASSET_H_
#define _SE_ASSET_H_
#include "../Core/Addressable/Addressable.h"

namespace SE
{
	class AAsset : public SAddressable
	{
	public:
		AAsset();
		AAsset(const std::string& serializedFilePath);
		AAsset(const AAsset& other);
		virtual ~AAsset();

		// Initialize asset with the serialized file.
		void Load(const std::string& serializedFilePath);

		// Initialize asset by importing data from raw file.
		void Import(const std::string& rawFilePath);
		// Update data of asset through current raw file.
		void Reimport();

		template<typename DataType>
		DataType GetData()
		{
			return std::any_cast<DataType>(this->Data);
		}

	protected:
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

	private:
		static std::string ValidateCategory(std::string fileExtension);
		static std::any ImportData(std::string filePath, std::string category);

	public:
		// Asset Categories Enum :
		static std::string Texture;
		static std::string StaticMesh;
	};
}

#endif