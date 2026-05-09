#include <Core.h>
#include "../../../PipelineState/PipelineState.h"
#include "../../../RenderGroup/RenderGroup.h"
#include "../../../Framebuffer/Framebuffer.h"
#include "AlbedoPass.h"

namespace SE
{
	GAlbedoPass::GAlbedoPass() : GRenderQueuePass()
	{

	}

	GAlbedoPass::GAlbedoPass(const std::string& name) : GRenderQueuePass()
	{
		this->Initialize(name);
	}

	GAlbedoPass::GAlbedoPass(const GAlbedoPass & other) : GRenderQueuePass(other)
	{

	}

	GAlbedoPass::~GAlbedoPass()
	{

	}

	void GAlbedoPass::Initialize(const std::string & name)
	{
		this->RenderPassName = name;

		auto& GBufferInflow = GInflow::Create("GBufferFramebuffer", this->FramebufferPackage);
		this->AddInflow(GBufferInflow);
		this->AddOutflow(GOutflow::Create("GBufferFramebuffer", GBufferInflow));

		this->AddApplicable(SPipelineStateRegistry::GetInstance(GRenderGroup::ALBEDO_GROUP));
	}

	void GAlbedoPass::Execute()
	{
		auto& GBufferFramebuffer = SFramebufferRegistry::GetInstance(this->FramebufferPackage.GetResourceIdentifier().GetUUID());
		GBufferFramebuffer->SetCurrentBuffer(0);

		GRenderQueuePass::Execute();
	}
}
