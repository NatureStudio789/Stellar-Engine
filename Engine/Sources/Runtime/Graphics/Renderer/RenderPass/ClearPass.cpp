#include <Core.h>
#include "../../Framebuffer/Framebuffer.h"
#include "ClearPass.h"

namespace SE
{
	GClearPass::GClearPass()
	{

	}

	GClearPass::GClearPass(const std::string& name) : GRenderPass(name)
	{
		
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
		
		Framebuffer->Begin();
		Framebuffer->Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
		Framebuffer->End();
	}
}
