#include <Core.h>
#include "../../../PipelineState/PipelineState.h"
#include "../../../Framebuffer/Framebuffer.h"
#include "MetallicPass.h"

namespace SE
{
	GMetallicPass::GMetallicPass() : GRenderQueuePass()
	{

	}

	GMetallicPass::GMetallicPass(const std::string& name) : GRenderQueuePass()
	{
		this->Initialize(name);
	}

	GMetallicPass::GMetallicPass(const GMetallicPass & other) : GRenderQueuePass(other)
	{

	}

	GMetallicPass::~GMetallicPass()
	{

	}

	void GMetallicPass::Initialize(const std::string & name)
	{
		this->RenderPassName = name;

		auto& GBufferInflow = GInflow::Create("GBufferFramebuffer", this->FramebufferPackage);
		this->AddInflow(GBufferInflow);
		this->AddOutflow(GOutflow::Create("GBufferFramebuffer", GBufferInflow));

		this->AddApplicable(SPipelineStateRegistry::GetInstance(GRenderGroup::METALLIC_GROUP));
	}

	void GMetallicPass::Execute()
	{
		auto& GBufferFramebuffer = SFramebufferRegistry::GetInstance(this->FramebufferPackage.GetResourceIdentifier().GetUUID());
		GBufferFramebuffer->SetCurrentBuffer(1);

		GRenderQueuePass::Execute();
	}
}
