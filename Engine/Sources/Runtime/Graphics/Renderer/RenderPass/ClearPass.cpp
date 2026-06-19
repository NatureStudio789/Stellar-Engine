#include <Core.h>
#include "../../Framebuffer/Framebuffer.h"
#include "ClearPass.h"

namespace SE
{
	GClearPass::GClearPass()
	{

	}

	GClearPass::GClearPass(const std::string& name, std::vector<unsigned int> multipleRenderTargetClearingList) : GRenderPass(name)
	{
		auto& FramebufferInflow = GInflow::Create("ClearingFramebuffer", this->ClearingFramebufferPackage);
		this->AddInflow(FramebufferInflow);
		this->AddOutflow(GOutflow::Create("ClearingFramebuffer", FramebufferInflow));

		this->MultipleRenderTargetClearingList = multipleRenderTargetClearingList;
	}

	GClearPass::GClearPass(const GClearPass & other)
	{
		this->ClearingFramebufferPackage = other.ClearingFramebufferPackage;
	}

	GClearPass::~GClearPass()
	{

	}

	void GClearPass::Execute()
	{
		auto& Framebuffer = SFramebufferRegistry::GetInstance(this->ClearingFramebufferPackage.GetResourceIdentifier().GetUUID());

		if (this->MultipleRenderTargetClearingList.empty())
		{
			Framebuffer->Begin();
			Framebuffer->Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
			Framebuffer->End();
		}
		else
		{
			// Batch all MRT buffer transitions into single barrier calls.
			Framebuffer->BeginMultiple(this->MultipleRenderTargetClearingList);

			for (const auto index : this->MultipleRenderTargetClearingList)
			{
				Framebuffer->SetCurrentBuffer(index);
				Framebuffer->Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
			}

			Framebuffer->EndMultiple(this->MultipleRenderTargetClearingList);
		}
	}
}
