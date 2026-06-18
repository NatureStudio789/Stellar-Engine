#ifndef _SE_STATICMESH_H_
#define _SE_STATICMESH_H_
#include "MeshNode.h"

namespace SE
{
	class GStaticMesh
	{
	public:
		struct Data
		{
			Data() = default;
			Data(const Data&) = default;

			std::string FilePath;
			std::string FileDirectory;

			std::vector<GMeshItem::Data> ItemDataList;
			GMeshNode::Data RootNodeData;
		};

	public:
		GStaticMesh();
		GStaticMesh(const std::string& filePath);
		GStaticMesh(const Data& data);
		GStaticMesh(const GStaticMesh& other);
		~GStaticMesh();

		void Initialize(const std::string& filePath);
		void Initialize(const Data& data);

		void SetTransform(const STransform& transform);
		void SetAccumulatedMatrix(glm::mat4x4 accumulatedMatrix);

		void Submit(const std::string& channel);

		std::shared_ptr<GMeshNode> GetRootNode();

		static Data Load(const std::string& filePath);
		static Data Load(const std::string& filePath, char* fileData, unsigned long long dataSize);

	private:
		void LinkTechnique(const std::string& rendererName);

		std::shared_ptr<GMeshNode> BuildMeshNode(const GMeshNode::Data& data);

		static GMeshItem::Data ParseMeshItem(const aiScene* scene, aiMesh* mesh, const std::string& filePath);
		static GMeshNode::Data ParseMeshNode(const aiScene* scene, aiNode* node, const std::vector<GMeshItem::Data>& itemDataList);

		static void LoadTexture(aiMaterial* material, std::shared_ptr<GStandardMaterial> outputMaterial,
			aiTextureType type, std::string modelFileDirectory);
		static void LoadMetallicTexture(aiMaterial* material, std::shared_ptr<GStandardMaterial> outputMaterial, std::string modelFileDirectory);

		std::string MeshFilePath;
		std::string MeshFileDirectory;

		std::shared_ptr<GMeshNode> MeshRootNode;
		std::vector<std::shared_ptr<GMeshItem>> MeshItemList;

		glm::mat4x4 AccumulatedMatrix;
	};
}

#endif