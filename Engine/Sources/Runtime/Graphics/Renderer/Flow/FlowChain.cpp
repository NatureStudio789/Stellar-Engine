#include <Core.h>
#include "FlowChain.h"

namespace SE
{
	GFlowChain::GFlowChain()
	{
		this->IsDynamic = false;
		this->FlowList.clear();
	}

	GFlowChain::GFlowChain(const GFlowChain& other)
	{
		this->FlowList = other.FlowList;
		this->IsDynamic = other.IsDynamic;
	}

	GFlowChain::~GFlowChain()
	{
		this->FlowList.clear();
		this->IsDynamic = false;
	}

	void GFlowChain::Link()
	{
		if (this->FlowList.empty() || !this->FlowList[0].first->IsSource())
		{
			SMessageHandler::Instance->SetFatal("Graphics", "This flow chain is invalid!");
		}

		for (UINT i = 0; i < (UINT)this->FlowList.size(); i++)
		{
			if (!this->FlowList[i].first->IsSource())
			{
				this->FlowList[i].first->ResourcePackage = this->FlowList[(i - 1)].second->ResourcePackage;
			}

			this->FlowList[i].second->Apply(this->FlowList[i].first);
		}
	}

	const bool& GFlowChain::IsChainDynamic() const noexcept
	{
		return this->IsDynamic;
	}
}
