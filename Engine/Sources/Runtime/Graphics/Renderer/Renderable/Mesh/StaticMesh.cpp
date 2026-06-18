#include <Core.h>
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

	GStaticMesh::GStaticMesh(const GStaticMesh & other)
	{
		this->MeshFilePath = other.MeshFilePath;
		this->MeshFileDirectory = other.MeshFileDirectory;

		this->MeshRootNode = other.MeshRootNode;
		this->MeshItemList = other.MeshItemList;

		this->AccumulatedMatrix = other.AccumulatedMatrix;
	}

	GStaticMesh::~GStaticMesh()
	{
		
	}

	void GStaticMesh::Initialize(const std::string & filePath)
	{
		this->MeshFilePath = filePath;
		this->MeshFileDirectory = std::filesystem::path(filePath).parent_path().string();

		const auto& Data = Load(filePath);

		for (const auto& itemData : Data.ItemDataList)
		{
			this->MeshItemList.push_back(std::make_shared<GMeshItem>(itemData));
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
			this->MeshItemList.push_back(std::make_shared<GMeshItem>(itemData));
		}

		this->MeshRootNode = this->BuildMeshNode(data.RootNodeData);
		this->AccumulatedMatrix = glm::mat4x4(1.0f);

		this->LinkTechnique("MainDeferredRenderer");
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

	GStaticMesh::Data GStaticMesh::Load(const std::string& filePath)
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
			MeshItemDataList.push_back(ParseMeshItem(MeshScene, MeshScene->mMeshes[i], MeshData.FilePath));
		}

		MeshData.ItemDataList = MeshItemDataList;

		MeshData.RootNodeData = ParseMeshNode(MeshScene, MeshScene->mRootNode, MeshItemDataList);

		return MeshData;
	}

	GStaticMesh::Data GStaticMesh::Load(const std::string& filePath, char* fileData, unsigned long long dataSize)
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
			MeshItemDataList.push_back(ParseMeshItem(MeshScene, MeshScene->mMeshes[i], MeshData.FilePath));
		}

		MeshData.ItemDataList = MeshItemDataList;

		MeshData.RootNodeData = ParseMeshNode(MeshScene, MeshScene->mRootNode, MeshItemDataList);

		return MeshData;
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
			Node->AddChild(BuildMeshNode(childNodeData));
		}

		return Node;
	}

	GMeshItem::Data GStaticMesh::ParseMeshItem(const aiScene* scene, aiMesh* mesh, const std::string& filePath)
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
			LoadTexture(Material, ItemMaterial, aiTextureType_DIFFUSE, FileDirectory);
			LoadMetallicTexture(Material, ItemMaterial, FileDirectory);
			LoadTexture(Material, ItemMaterial, aiTextureType_SHININESS, FileDirectory);
			LoadTexture(Material, ItemMaterial, aiTextureType_NORMALS, FileDirectory);

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
		aiTextureType type, std::string modelFileDirectory)
	{
		if (material->GetTextureCount(type))
		{
			aiString WrittenPath;
			material->GetTexture(type, 0, &WrittenPath);
			
			std::string FilePath;
			if (std::filesystem::exists(WrittenPath.C_Str()))
			{
				FilePath = WrittenPath.C_Str();
			}
			else
			{
				auto FinalPath = std::filesystem::path(modelFileDirectory) / std::filesystem::path(WrittenPath.C_Str());
				FilePath = FinalPath.string();
			}

			switch (type)
			{
				case aiTextureType_DIFFUSE:
				{
					std::shared_ptr<GTexture> Texture = std::make_shared<GTexture>(FilePath, 0, GRenderGroup::ALBEDO_GROUP);
					outputMaterial->SetAlbedo(Texture);
					break;
				}

				case aiTextureType_NORMALS:
				{
					std::shared_ptr<GTexture> Texture = std::make_shared<GTexture>(FilePath, 3, GRenderGroup::NORMAL_GROUP);
					outputMaterial->SetNormalTexture(Texture);
					break;
				}

				case aiTextureType_SHININESS:
				{
					std::shared_ptr<GTexture> Texture = std::make_shared<GTexture>(FilePath, 2, GRenderGroup::ROUGHNESS_GROUP);
					outputMaterial->SetRoughness(Texture);
					break;
				}

				//case aiTextureType_AMBIENT_OCCLUSION:
				//{
				//	outMat->SetAoTexture(AssetName);
				//	break;
				//}
			}
		}
	}

	void GStaticMesh::LoadMetallicTexture(aiMaterial* material, std::shared_ptr<GStandardMaterial> outputMaterial, std::string modelFileDirectory)
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

					std::string FilePath;
					if (std::filesystem::exists(WrittenPath))
					{
						FilePath = WrittenPath;
					}
					else
					{
						auto FinalPath = std::filesystem::path(modelFileDirectory) / std::filesystem::path(WrittenPath);
						FilePath = FinalPath.string();
					}

					MetallicTexture = std::make_shared<GTexture>(FilePath, 1, GRenderGroup::METALLIC_GROUP);
					outputMaterial->SetMetallic(MetallicTexture);

					return;
				}
			}
		}
	}
}
