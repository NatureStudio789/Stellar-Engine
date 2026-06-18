#ifndef _SE_MESHNODE_H_
#define _SE_MESHNODE_H_
#include "MeshItem.h"

#include "../../../../Core/Addressable/Addressable.h"

namespace SE
{
	class GMeshNode : public SAddressable
	{
	public:
		struct Data
		{
			Data() = default;
			Data(const Data&) = default;

			std::string Name;

			std::vector<UINT> ItemDataIndexList;

			std::vector<Data> ChildrenList;
		};

	public:
		GMeshNode();
		GMeshNode(const std::string& name, 
			const std::vector<std::shared_ptr<GMeshItem>>& itemList);
		GMeshNode(const GMeshNode& other);
		~GMeshNode() override;

		void SetItemList(const std::vector<std::shared_ptr<GMeshItem>>& itemList);
		void AddItem(std::shared_ptr<GMeshItem> item);

		void SetTransform(const STransform& transform);

		void Submit(const std::string& channel, glm::mat4x4 accumulatedMatrix);

		bool HasChildren() const noexcept;
		std::shared_ptr<GMeshNode> GetChild(const SUUID& id);
		std::shared_ptr<GMeshNode> GetChild(const std::string& name);

	private:
		void AddChild(std::shared_ptr<GMeshNode> child);

		std::vector<std::shared_ptr<GMeshNode>> ChildrenList;
		std::vector<std::shared_ptr<GMeshItem>> ItemList;

		STransform NodeTransform;

		friend class GStaticMesh;
	};
}

#endif