#include <Core.h>
#include "../Core/MessageHandler/MessageHandler.h"

#include "../Graphics/Image/Image.h"
#include "../Graphics/Renderer/Renderable/Mesh/StaticMesh.h"

#include "Asset.h"

namespace SE
{
	AAsset::AAsset() : SAddressable()
	{
		this->Category = "Unknown";
	}

	AAsset::AAsset(const std::string& serializedFilePath) : SAddressable()
	{
		this->Load(serializedFilePath);
	}

	AAsset::AAsset(const AAsset & other) : SAddressable(other)
	{
		this->Category = other.Category;

		this->FilePackage = other.FilePackage;

		this->RawFilePath = other.RawFilePath;
		this->RawFileExtension = other.RawFileExtension;

		this->SerializedFilePath = other.SerializedFilePath;
		this->IsSerialized = other.IsSerialized;

		this->Data = other.Data;
	}

	AAsset::~AAsset()
	{

	}

	void AAsset::Load(const std::string & serializedFilePath)
	{
		// TO DO: Deserialize the asset from serialized file...

		this->Data = ImportData(this->RawFilePath.string(), this->Category);
	}

	void AAsset::Import(const std::string& rawFilePath)
	{
		this->RawFilePath = rawFilePath;
		this->RawFileExtension = this->RawFilePath.extension().string();
		this->SetName(this->RawFilePath.stem().string());

		this->FilePackage = this->RawFilePath.parent_path().string();

		this->SerializedFilePath = this->FilePackage / this->RawFilePath.stem();
		this->SerializedFilePath += ".sasset";
		this->IsSerialized = false;

		this->Category = ValidateCategory(this->RawFileExtension);

		this->Data = ImportData(this->RawFilePath.string(), this->Category);

		this->Activate();

		// TO DO: Serialize the asset...
	}

	void AAsset::Reimport()
	{
		this->Data = ImportData(this->RawFilePath.string(), this->Category);
	}

	std::string AAsset::ValidateCategory(std::string fileExtension)
	{
		std::vector<std::string> SupportedTextureFileTypes =
		{
			".bmp", ".png", ".jpg", "jpeg", ".gif",
			".tif", ".tiff", ".wdp", ".hdp", ".jxr"
		};

		std::vector<std::string> SupportedStaticMeshFileTypes =
		{
			".fbx", ".gltf", ".glb", ".dae", 
			".obj", ".ply", ".stl", ".off"
		};

		for (const auto& textureFileType : SupportedTextureFileTypes)
		{
			if (fileExtension == textureFileType)
			{
				return Texture;
				break;
			}
		}

		for (const auto& staticMeshFileType : SupportedStaticMeshFileTypes)
		{
			if (fileExtension == staticMeshFileType)
			{
				return StaticMesh;
				break;
			}
		}

		return "Unknown";
	}

	std::any AAsset::ImportData(std::string filePath, std::string category)
	{
		if (category == Texture)
		{
			std::shared_ptr<GImage> TextureData = std::make_shared<GImage>(filePath);
			return std::any(TextureData);
		}
		else if (category == StaticMesh)
		{
			GStaticMesh::Data StaticMeshData = GStaticMesh::Load(filePath);
			return std::any(StaticMeshData);
		}
		else
		{
			SMessageHandler::Instance->SetFatal("Asset", "Failed to import asset data from an UNKNOWN category.");
			return std::any();
		}
	}


	std::string AAsset::Texture = "SA_Texture";
	std::string AAsset::StaticMesh = "SA_StaticMesh";
}
