#include <Core.h>
#include "../../../PipelineState/PipelineState.h"
#include "../../../Framebuffer/Framebuffer.h"
#include "NormalPass.h"

namespace SE
{
    GNormalPass::GNormalPass() : GRenderQueuePass()
    {
        
    }

    GNormalPass::GNormalPass(const std::string& name) : GRenderQueuePass()
    {
        this->Initialize(name);
    }

    GNormalPass::GNormalPass(const GNormalPass & other) : GRenderQueuePass(other)
    {

    }

    GNormalPass::~GNormalPass()
    {

    }

    void GNormalPass::Initialize(const std::string & name)
    {
        this->RenderPassName = name;

        auto GBufferInflow = GInflow::Create("GBufferFramebuffer", this->FramebufferPackage);
        this->AddInflow(GBufferInflow);
        this->AddOutflow(GOutflow::Create("GBufferFramebuffer", GBufferInflow));

        this->AddApplicable(SPipelineStateRegistry::GetInstance(GRenderGroup::NORMAL_GROUP));
    }

    void GNormalPass::Execute()
    {
        auto GBufferFramebuffer = SFramebufferRegistry::GetInstance(this->FramebufferPackage.GetResourceIdentifier().GetUUID());
        GBufferFramebuffer->SetCurrentBuffer(3);

        GRenderQueuePass::Execute();
    }
}
