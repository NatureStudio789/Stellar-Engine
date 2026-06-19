#include <Core.h>
#include "../../../PipelineState/PipelineState.h"
#include "../../../Framebuffer/Framebuffer.h"
#include "RoughnessPass.h"

namespace SE
{
    GRoughnessPass::GRoughnessPass() : GRenderQueuePass()
    {

    }

    GRoughnessPass::GRoughnessPass(const std::string& name) : GRenderQueuePass()
    {
        this->Initialize(name);
    }

    GRoughnessPass::GRoughnessPass(const GRoughnessPass & other) : GRenderQueuePass(other)
    {

    }

    GRoughnessPass::~GRoughnessPass()
    {

    }

    void GRoughnessPass::Initialize(const std::string & name)
    {
        this->RenderPassName = name;

        auto& GBufferInflow = GInflow::Create("GBufferFramebuffer", this->FramebufferPackage);
        this->AddInflow(GBufferInflow);
        this->AddOutflow(GOutflow::Create("GBufferFramebuffer", GBufferInflow));

        this->AddApplicable(SPipelineStateRegistry::GetInstance(GRenderGroup::ROUGHNESS_GROUP));
    }

    void GRoughnessPass::Execute()
    {
        auto& GBufferFramebuffer = SFramebufferRegistry::GetInstance(this->FramebufferPackage.GetResourceIdentifier().GetUUID());
        GBufferFramebuffer->SetCurrentBuffer(2);

        GRenderQueuePass::Execute();
    }
}
