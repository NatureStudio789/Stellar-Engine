#include <Core.h>
#include "../Core/MessageHandler/MessageHandler.h"

#include "../Graphics/Image/Image.h"
#include "../Graphics/Renderer/Renderable/Mesh/StaticMesh.h"

#include "../Function/Serializer/AssetSerializer.h"

#include "Asset.h"

namespace SE
{
	AAsset::AAsset(const std::string& belongingLoaderName) : SAddressable()
	{
		this->BelongingLoaderName = belongingLoaderName;

		this->Category = "Unknown";
	}

	AAsset::AAsset(const std::string& serializedFilePath, const std::string& belongingLoaderName) : SAddressable()
	{
		this->BelongingLoaderName = belongingLoaderName;

		this->Load(serializedFilePath);
	}

	AAsset::AAsset(const AAsset & other) : SAddressable(other)
	{
		this->BelongingLoaderName = other.BelongingLoaderName;

		this->Category = other.Category;

		this->FilePackage = other.FilePackage;

		this->RawFilePath = other.RawFilePath;
		this->RawFileExtension = other.RawFileExtension;

		this->SerializedFilePath = other.SerializedFilePath;
		this->IsSerialized = other.IsSerialized;

		this->Data = other.Data;

		this->BindingParent = other.BindingParent;
	}

	AAsset::~AAsset()
	{

	}

	void AAsset::Load(const std::string & serializedFilePath)
	{
		FAssetSerializer::Import(serializedFilePath, this);

		this->Data = ImportData(this->RawFilePath.string(), this->Category, this->BelongingLoaderName);
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

		this->Data = ImportData(this->RawFilePath.string(), this->Category, this->BelongingLoaderName);

		this->Activate();

		FAssetSerializer::Export(this->SerializedFilePath.string(), this);
		this->IsSerialized = true;
	}

	void AAsset::AddBindingParent(SAssetizable* parent)
	{
		this->BindingParent.push_back(parent);
	}

	const std::string& AAsset::GetBelongingLoaderName() const noexcept
	{
		return this->BelongingLoaderName;
	}

	const std::string& AAsset::GetCategory() const noexcept
	{
		return this->Category;
	}

	const std::string& AAsset::GetFilePackage() const noexcept
	{
		return this->FilePackage;
	}

	std::string AAsset::GetRawFilePath() const noexcept
	{
		return this->RawFilePath.string();
	}

	std::string AAsset::GetSerializedFilePath() const noexcept
	{
		return this->SerializedFilePath.string();
	}

	bool AAsset::GetSerialized() const noexcept
	{
		return this->IsSerialized;
	}

	void AAsset::ResetData(const std::any& data)
	{
		this->Data = data;

		for (auto parent : this->BindingParent)
		{
			parent->ReinitializeFromAsset(this);
		}
	}

	void AAsset::ResetPackage(const std::filesystem::path& package)
	{
		this->FilePackage = package.string();

		std::filesystem::path FileName = this->RawFilePath.filename();
		this->RawFilePath = package / FileName;
		if (this->IsSerialized)
		{
			this->SerializedFilePath = package / std::filesystem::path(FileName.stem().string() + ".sasset");

			FAssetSerializer::Export(this->SerializedFilePath.string(), this);
		}
	}

	std::string AAsset::ValidateCategory(std::string fileExtension)
	{
		std::vector<std::string> SupportedTextureFileTypes =
		{
			".bmp", ".png", ".jpg", ".jpeg", ".gif",
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

	std::any AAsset::ImportData(std::string filePath, std::string category, std::string belongingLoaderName)
	{
		if (category == Texture)
		{
			std::shared_ptr<GImage> TextureData = std::make_shared<GImage>(filePath);
			return std::any(TextureData);
		}
		else if (category == StaticMesh)
		{
			GStaticMesh::Data StaticMeshData = GStaticMesh::Load(filePath, true, belongingLoaderName);
			return std::any(StaticMeshData);
		}
		else
		{
			SMessageHandler::Instance->SetFatal("Asset", "Failed to import asset data from an UNKNOWN category.");
			return std::any();
		}
	}


	const std::string AAsset::Texture = "SA_Texture";
	const std::string AAsset::StaticMesh = "SA_StaticMesh";
}
