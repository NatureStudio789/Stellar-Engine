#include <Core.h>
#include "MeshNode.h"

namespace SE
{
	GMeshNode::GMeshNode() : SAddressable()
	{
		this->NodeTransform = {};

		this->Activate();
	}

	GMeshNode::GMeshNode(const std::string& name, const std::vector<std::shared_ptr<GMeshItem>>& itemList) : SAddressable()
	{
		this->SetName(name);

		this->ItemList = itemList;

		this->Activate();
	}

	GMeshNode::GMeshNode(const GMeshNode & other) : SAddressable(other)
	{
		this->ChildrenList = other.ChildrenList;
		this->ItemList = other.ItemList;

		this->NodeTransform = other.NodeTransform;
	}

	GMeshNode::~GMeshNode()
	{
		this->ChildrenList.clear();
		this->ItemList.clear();

		this->NodeTransform = {};
	}

	void GMeshNode::SetItemList(const std::vector<std::shared_ptr<GMeshItem>>& itemList)
	{
		this->ItemList = itemList;
	}

	void GMeshNode::AddItem(std::shared_ptr<GMeshItem> item)
	{
		this->ItemList.push_back(item);
	}

	void GMeshNode::SetTransform(const STransform & transform)
	{
		this->NodeTransform = transform;
	}

	void GMeshNode::Submit(const std::string & channel, glm::mat4x4 accumulatedMatrix)
	{
		glm::mat4x4 WorldMatrix = this->NodeTransform.GetMatrix();
		glm::mat4x4 PassingAccumulatedMatrix = WorldMatrix * accumulatedMatrix;

		for (auto& item : this->ItemList)
		{
			item->SetTransform(this->NodeTransform);
			item->SetAccumulatedMatrix(accumulatedMatrix);

			item->Submit(channel);
		}

		for (auto& child : this->ChildrenList)
		{
			child->Submit(channel, PassingAccumulatedMatrix);
		}
	}

	bool GMeshNode::HasChildren() const noexcept
	{
		return !this->ChildrenList.empty();
	}

	std::shared_ptr<GMeshNode> GMeshNode::GetChild(const SUUID& id)
	{
		for (auto& child : this->ChildrenList)
		{
			if (child->GetUUID() == id)
			{
				return child;
			}
		}

		SMessageHandler::Instance->SetFatal("Graphics",
			std::format("Failed to find child with id : '{}' in mesh node named '{}'", (const std::string&)id, this->GetName()));
		return null;
	}

	std::shared_ptr<GMeshNode> GMeshNode::GetChild(const std::string& name)
	{
		for (auto& child : this->ChildrenList)
		{
			if (child->GetName() == name)
			{
				return child;
			}
		}

		SMessageHandler::Instance->SetFatal("Graphics",
			std::format("Failed to find child named '{}' in mesh node named '{}'", name, this->GetName()));
		return null;
	}

	void GMeshNode::AddChild(std::shared_ptr<GMeshNode> child)
	{
		this->ChildrenList.push_back(child);
	}
}
