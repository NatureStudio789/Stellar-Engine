#include <Core.h>
#include "../../../PipelineState/PipelineState.h"
#include "../../../Framebuffer/Framebuffer.h"
#include "PositionPass.h"

namespace SE
{
	GPositionPass::GPositionPass() : GRenderQueuePass()
	{

	}

	GPositionPass::GPositionPass(const std::string& name) : GRenderQueuePass()
	{
		this->Initialize(name);
	}

	GPositionPass::GPositionPass(const GPositionPass & other) : GRenderQueuePass(other)
	{

	}

	GPositionPass::~GPositionPass()
	{

	}

	void GPositionPass::Initialize(const std::string & name)
	{
		this->RenderPassName = name;

		auto& GBufferInflow = GInflow::Create("GBufferFramebuffer", this->FramebufferPackage);
		this->AddInflow(GBufferInflow);
		this->AddOutflow(GOutflow::Create("GBufferFramebuffer", GBufferInflow));

		this->AddApplicable(SPipelineStateRegistry::GetInstance(GRenderGroup::POSITION_GROUP));
	}

	void GPositionPass::Execute()
	{
		auto& GBufferFramebuffer = SFramebufferRegistry::GetInstance(this->FramebufferPackage.GetResourceIdentifier().GetUUID());
		GBufferFramebuffer->SetCurrentBuffer(4);

		GRenderQueuePass::Execute();
	}
}
