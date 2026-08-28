#include <Core.h>
#include "../../../../Asset/Loader/AssetLoader.h"
#include "../../../Material/StandardMaterial.h"
#include "StaticMesh.h"

namespace SE
{
	GStaticMesh::GStaticMesh()
	{
		this->MeshFilePath = "";
		this->MeshFileDirectory = "";

		this->MeshRootNode = null;
		this->MeshItemList.clear();
		this->MeshItemAABBList.clear();

		this->AccumulatedMatrix = glm::mat4x4(1.0f);
	}

	GStaticMesh::GStaticMesh(const std::string& filePath)
	{
		this->Initialize(filePath);
	}

	GStaticMesh::GStaticMesh(const Data & data)
	{
		this->Initialize(data);
	}

	GStaticMesh::GStaticMesh(const SUUID& assetId, const std::string& assetLoaderName)
	{
		this->Initialize(SAssetLoaderRegistry::GetInstance(assetLoaderName)->GetAsset(assetId));
	}

	GStaticMesh::GStaticMesh(std::shared_ptr<AAsset> asset)
	{
		this->Initialize(asset);
	}

	GStaticMesh::GStaticMesh(const GStaticMesh & other) : SAssetizable(other)
	{
		this->MeshFilePath = other.MeshFilePath;
		this->MeshFileDirectory = other.MeshFileDirectory;

		this->MeshRootNode = other.MeshRootNode;
		this->MeshItemList = other.MeshItemList;
		this->MeshItemAABBList = other.MeshItemAABBList;

		this->AccumulatedMatrix = other.AccumulatedMatrix;
	}

	void GStaticMesh::Initialize(const std::string & filePath)
	{
		this->MeshFilePath = filePath;
		this->MeshFileDirectory = std::filesystem::path(filePath).parent_path().string();

		const auto& Data = Load(filePath, false, "");

		for (const auto& itemData : Data.ItemDataList)
		{
			auto MeshItem = std::make_shared<GMeshItem>(itemData);
			this->MeshItemList.push_back(MeshItem);
			this->MeshItemAABBList.push_back(MeshItem->GetAABB());
		}

		this->MeshRootNode = this->BuildMeshNode(Data.RootNodeData);
		this->AccumulatedMatrix = glm::mat4x4(1.0f);

		this->LinkTechnique("MainDeferredRenderer");
	}

	void GStaticMesh::Initialize(const Data & data)
	{
		this->MeshFilePath = data.FilePath;
		this->MeshFileDirectory = data.FileDirectory;

		for (const auto& itemData : data.ItemDataList)
		{
			auto MeshItem = std::make_shared<GMeshItem>(itemData);
			this->MeshItemList.push_back(MeshItem);
			this->MeshItemAABBList.push_back(MeshItem->GetAABB());
		}

		this->MeshRootNode = this->BuildMeshNode(data.RootNodeData);
		this->AccumulatedMatrix = glm::mat4x4(1.0f);

		this->LinkTechnique("MainDeferredRenderer");
	}

	void GStaticMesh::Initialize(std::shared_ptr<AAsset> asset)
	{
		asset->AddBindingParent(this);

		if (asset->GetCategory() != AAsset::StaticMesh)
		{
			SMessageHandler::Instance->SetFatal("Graphics",
				std::format("Failed to initialize static mesh through the asset in a wrong category : '{}'!", asset->GetCategory()));
		}

		this->Initialize(asset->GetData<Data>());
		this->IsAssetized = true;
		this->AssetLoaderName = asset->GetBelongingLoaderName();
	}

	void GStaticMesh::SetTransform(const STransform & transform)
	{
		this->MeshRootNode->SetTransform(transform);
	}

	void GStaticMesh::SetAccumulatedMatrix(glm::mat4x4 accumulatedMatrix)
	{
		this->AccumulatedMatrix = accumulatedMatrix;
	}

	void GStaticMesh::Submit(const std::string & channel)
	{
		this->MeshRootNode->Submit(channel, this->AccumulatedMatrix);
	}

	std::shared_ptr<GMeshNode> GStaticMesh::GetRootNode()
	{
		return this->MeshRootNode;
	}

	const std::vector<std::shared_ptr<SAABB>>& GStaticMesh::GetItemAABBList() const noexcept
	{
		return this->MeshItemAABBList;
	}

	GStaticMesh::Data GStaticMesh::Load(const std::string& filePath, bool isAssetized, std::string assetLoaderName)
	{
		Data MeshData;
		MeshData.FilePath = filePath;
		MeshData.FileDirectory = std::filesystem::path(filePath).parent_path().string();

		Assimp::Importer Importer;
		const aiScene* MeshScene = Importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
		if (!MeshScene || MeshScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !MeshScene->mRootNode)
		{
			SMessageHandler::Instance->SetFatal("Graphics", std::format("Failed to load static mesh file : '{}'", filePath));
		}

		std::vector<GMeshItem::Data> MeshItemDataList;
		for (UINT i = 0; i < MeshScene->mNumMeshes; i++)
		{
			MeshItemDataList.push_back(ParseMeshItem(MeshScene, MeshScene->mMeshes[i], MeshData.FilePath, isAssetized, assetLoaderName));
		}

		MeshData.ItemDataList = MeshItemDataList;

		MeshData.RootNodeData = ParseMeshNode(MeshScene, MeshScene->mRootNode, MeshItemDataList);

		return MeshData;
	}

	GStaticMesh::Data GStaticMesh::Load(const std::string& filePath, char* fileData, unsigned long long dataSize, bool isAssetized, std::string assetLoaderName)
	{
		Data MeshData;
		MeshData.FilePath = filePath;
		MeshData.FileDirectory = std::filesystem::path(filePath).parent_path().string();

		Assimp::Importer Importer;
		const aiScene* MeshScene = Importer.ReadFileFromMemory(fileData, dataSize, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
		if (!MeshScene || MeshScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !MeshScene->mRootNode)
		{
			SMessageHandler::Instance->SetFatal("Graphics", std::format("Failed to load static mesh file : '{}'", filePath));
		}

		std::vector<GMeshItem::Data> MeshItemDataList;
		for (UINT i = 0; i < MeshScene->mNumMeshes; i++)
		{
			MeshItemDataList.push_back(ParseMeshItem(MeshScene, MeshScene->mMeshes[i], MeshData.FilePath, isAssetized, assetLoaderName));
		}

		MeshData.ItemDataList = MeshItemDataList;

		MeshData.RootNodeData = ParseMeshNode(MeshScene, MeshScene->mRootNode, MeshItemDataList);

		return MeshData;
	}

	void GStaticMesh::ReinitializeFromAsset(AAsset* asset)
	{
		if (asset->GetCategory() != AAsset::StaticMesh)
		{
			SMessageHandler::Instance->SetFatal("Graphics",
				std::format("Failed to initialize static mesh through the asset in a wrong category : '{}'!", asset->GetCategory()));
		}

		for (auto meshItem : this->MeshItemList)
		{
			meshItem.reset();
			meshItem = null;
		}
		this->MeshItemList.clear();

		for (auto meshItemAABB : this->MeshItemAABBList)
		{
			meshItemAABB.reset();
			meshItemAABB = null;
		}
		this->MeshItemAABBList.clear();

		auto AssetData = asset->GetData<Data>();

		for (const auto& itemData : AssetData.ItemDataList)
		{
			auto MeshItem = std::make_shared<GMeshItem>(itemData);
			this->MeshItemList.push_back(MeshItem);
			this->MeshItemAABBList.push_back(MeshItem->GetAABB());
		}

		this->LinkTechnique("MainDeferredRenderer");

		auto RootNode = this->BuildMeshNode(AssetData.RootNodeData);
		this->MeshRootNode = RootNode;
	}

	void GStaticMesh::LinkTechnique(const std::string& rendererName)
	{
		for (auto& meshItem : this->MeshItemList)
		{
			meshItem->LinkTechnique(rendererName);
		}
	}

	std::shared_ptr<GMeshNode> GStaticMesh::BuildMeshNode(const GMeshNode::Data& data)
	{
		std::shared_ptr<GMeshNode> Node;

		std::vector<std::shared_ptr<GMeshItem>> ItemList;
		for (auto& index : data.ItemDataIndexList)
		{
			ItemList.push_back(this->MeshItemList[index]);
		}

		Node = std::make_shared<GMeshNode>(data.Name, ItemList);

		for (auto& childNodeData : data.ChildrenList)
		{
			Node->AddChild(this->BuildMeshNode(childNodeData));
		}

		return Node;
	}

	GMeshItem::Data GStaticMesh::ParseMeshItem(const aiScene* scene, aiMesh* mesh, const std::string& filePath, bool isAssetized, std::string assetLoaderName)
	{
		GMeshItem::Data ItemData;
		ItemData.Name = mesh->mName.C_Str();
		
		std::vector<GMeshItem::Vertex> Vertices;
		std::vector<unsigned int> Indices;

		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			GMeshItem::Vertex Vertex = {};

			Vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			Vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			if (mesh->mTextureCoords[0])
			{
				Vertex.TextureCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			}

			Vertices.push_back(Vertex);
		}

		for (UINT i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				Indices.push_back(face.mIndices[j]);
			}
		}

		ItemData.Vertices = Vertices;
		ItemData.Indices = Indices;
		
		aiMaterial* Material = scene->mMaterials[mesh->mMaterialIndex];
		std::string FileName = std::filesystem::path(filePath).filename().string();
		std::string StaticMeshName = FileName.substr(0, FileName.find_first_of('.'));
		if (!SMaterialRegistry::HasInstance((StaticMeshName + "_Mat_" + Material->GetName().C_Str())))
		{
			std::shared_ptr<GStandardMaterial> ItemMaterial = std::make_shared<GStandardMaterial>(
				(StaticMeshName + "_Mat_" + Material->GetName().C_Str()));

			std::string FileDirectory = std::filesystem::path(filePath).parent_path().string();
			LoadTexture(Material, ItemMaterial, aiTextureType_DIFFUSE, FileDirectory, isAssetized, assetLoaderName);
			LoadMetallicTexture(Material, ItemMaterial, FileDirectory, isAssetized, assetLoaderName);
			LoadTexture(Material, ItemMaterial, aiTextureType_SHININESS, FileDirectory, isAssetized, assetLoaderName);
			LoadTexture(Material, ItemMaterial, aiTextureType_NORMALS, FileDirectory, isAssetized, assetLoaderName);

			SMaterialRegistry::Register(ItemMaterial);
		}

		ItemData.MaterialId = SMaterialRegistry::GetInstance((StaticMeshName + "_Mat_" + Material->GetName().C_Str()))->GetUUID();

		return ItemData;
	}

	GMeshNode::Data GStaticMesh::ParseMeshNode(const aiScene* scene, aiNode* node, const std::vector<GMeshItem::Data>& itemDataList)
	{
		GMeshNode::Data NodeData;

		std::vector<UINT> NodeItemIndexList;
		for (UINT i = 0; i < node->mNumMeshes; i++)
		{
			NodeItemIndexList.push_back(node->mMeshes[i]);
		}

		NodeData.Name = node->mName.C_Str();
		NodeData.ItemDataIndexList = NodeItemIndexList;

		for (UINT i = 0; i < node->mNumChildren; i++)
		{
			NodeData.ChildrenList.push_back(ParseMeshNode(scene, node->mChildren[i], itemDataList));
		}

		return NodeData;
	}

	void GStaticMesh::LoadTexture(aiMaterial* material, std::shared_ptr<GStandardMaterial> outputMaterial, 
		aiTextureType type, std::string modelFileDirectory, bool isAssetized, std::string assetLoaderName)
	{
		if (material->GetTextureCount(type))
		{
			aiString WrittenPath;
			material->GetTexture(type, 0, &WrittenPath);
			
			std::filesystem::path RawTextureFilePath;
			if (std::filesystem::exists(WrittenPath.C_Str()))
			{
				RawTextureFilePath = WrittenPath.C_Str();
			}
			else
			{
				auto FinalPath = std::filesystem::path(modelFileDirectory) / std::filesystem::path(WrittenPath.C_Str());
				RawTextureFilePath = FinalPath;
			}

			if (isAssetized)
			{
				std::filesystem::path TextureSerializedFilePath = (RawTextureFilePath.parent_path() / RawTextureFilePath.stem()).string() + ".sasset";
				// The assets of Textures in material of Static Mesh must be registered in the same registry of the Static Mesh's.
				auto TextureAsset = SAssetLoaderRegistry::GetInstance(assetLoaderName)->GetAsset(TextureSerializedFilePath);

				switch (type)
				{
					case aiTextureType_DIFFUSE:
					{
						std::shared_ptr<GTexture> Texture = std::make_shared<GTexture>(TextureAsset);
						Texture->SetRootParameterIndex(0, GRenderGroup::ALBEDO_GROUP);
						outputMaterial->SetAlbedo(Texture);
						break;
					}

					case aiTextureType_NORMALS:
					{
						std::shared_ptr<GTexture> Texture = std::make_shared<GTexture>(TextureAsset);
						Texture->SetRootParameterIndex(3, GRenderGroup::NORMAL_GROUP);
						outputMaterial->SetNormalTexture(Texture);
						break;
					}

					case aiTextureType_SHININESS:
					{
						std::shared_ptr<GTexture> Texture = std::make_shared<GTexture>(TextureAsset);
						Texture->SetRootParameterIndex(2, GRenderGroup::ROUGHNESS_GROUP);
						outputMaterial->SetRoughness(Texture);
						break;
					}
				}
			}
			else
			{
				switch (type)
				{
					case aiTextureType_DIFFUSE:
					{
						std::shared_ptr<GTexture> Texture = std::make_shared<GTexture>(RawTextureFilePath.string(), 0, GRenderGroup::ALBEDO_GROUP);
						outputMaterial->SetAlbedo(Texture);
						break;
					}

					case aiTextureType_NORMALS:
					{
						std::shared_ptr<GTexture> Texture = std::make_shared<GTexture>(RawTextureFilePath.string(), 3, GRenderGroup::NORMAL_GROUP);
						Texture->SetRootParameterIndex(3, GRenderGroup::NORMAL_GROUP);
						outputMaterial->SetNormalTexture(Texture);
						break;
					}

					case aiTextureType_SHININESS:
					{
						std::shared_ptr<GTexture> Texture = std::make_shared<GTexture>(RawTextureFilePath.string(), 2, GRenderGroup::ROUGHNESS_GROUP);
						outputMaterial->SetRoughness(Texture);
						break;
					}
				}
			}
		}
	}

	void GStaticMesh::LoadMetallicTexture(aiMaterial* material, std::shared_ptr<GStandardMaterial> outputMaterial, 
		std::string modelFileDirectory, bool isAssetized, std::string assetLoaderName)
	{
		std::shared_ptr<GTexture> MetallicTexture = null;

		for (UINT i = 0; i < material->mNumProperties; i++)
		{
			auto Properties = material->mProperties[i];

			if (Properties->mType == aiPTI_String)
			{
				std::string key = Properties->mKey.data;
				if (key == "$raw.ReflectionFactor|file")
				{
					UINT StringLength = *(UINT*)Properties->mData;
					std::string WrittenPath = { Properties->mData + 4, StringLength };

					std::filesystem::path RawTextureFilePath;
					if (std::filesystem::exists(WrittenPath))
					{
						RawTextureFilePath = WrittenPath;
					}
					else
					{
						RawTextureFilePath = std::filesystem::path(modelFileDirectory) / std::filesystem::path(WrittenPath);
					}

					if (isAssetized)
					{
						std::filesystem::path TextureSerializedFilePath = (RawTextureFilePath.parent_path() / RawTextureFilePath.stem()).string() + ".sasset";
						// The assets of Textures in material of Static Mesh must be registered in the same registry of the Static Mesh's.
						auto TextureAsset = SAssetLoaderRegistry::GetInstance(assetLoaderName)->GetAsset(TextureSerializedFilePath);

						MetallicTexture = std::make_shared<GTexture>(TextureAsset);
						MetallicTexture->SetRootParameterIndex(1, GRenderGroup::METALLIC_GROUP);

						outputMaterial->SetMetallic(MetallicTexture);
					}
					else
					{
						MetallicTexture = std::make_shared<GTexture>(RawTextureFilePath.string(), 1, GRenderGroup::METALLIC_GROUP);
						outputMaterial->SetMetallic(MetallicTexture);
					}

					return;
				}
			}
		}
	}
}
