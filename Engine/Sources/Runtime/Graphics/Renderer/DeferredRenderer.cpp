#include <Core.h>
#include "../Framebuffer/Framebuffer.h"
#include "DeferredRenderer.h"

namespace SE
{
    GDeferredRenderer::GDeferredRenderer(const std::string& name) : GRenderer(name)
    {
        this->GBufferFramebuffer = GFramebuffer::Create(glm::uvec2{ 2560, 1440 }, 4);
        this->GBufferFramebuffer->SetName("GBufferFramebuffer");
        SFramebufferRegistry::Register(this->GBufferFramebuffer);

        this->AddGlobalOutflow(GOutflow::Create("GBufferFramebuffer", this->GBufferFramebuffer->GetResourcePackage()));

        {
            auto ClearingPass = std::make_shared<GClearPass>("ClearingPass", std::vector<unsigned int>{ 0 });
            ClearingPass->SetLinkage("ClearingFramebuffer", "$.GBufferFramebuffer");

            this->AppendRenderPass(ClearingPass);
        }

        {
            auto AlbedoPass = std::make_shared<GAlbedoPass>("AlbedoBuffer");
            AlbedoPass->SetLinkage("GBufferFramebuffer", "ClearingPass.ClearingFramebuffer");

            this->AppendRenderPass(AlbedoPass);
        }

        this->Activate();
    }
}
