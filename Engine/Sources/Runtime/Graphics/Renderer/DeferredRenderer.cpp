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

        //{
        //    auto MetallicPass = std::make_shared<GMetallicPass>("MetallicBuffer");
        //    MetallicPass->SetLinkage("GBufferFramebuffer", "AlbedoBuffer.GBufferFramebuffer");

        //    this->AppendRenderPass(MetallicPass);
        //}

        //{
        //    auto RoughnessPass = std::make_shared<GRoughnessPass>("RoughnessBuffer");
        //    RoughnessPass->SetLinkage("GBufferFramebuffer", "MetallicBuffer.GBufferFramebuffer");

        //    this->AppendRenderPass(RoughnessPass);
        //}

        //{
        //    auto NormalPass = std::make_shared<GNormalPass>("NormalBuffer");
        //    NormalPass->SetLinkage("GBufferFramebuffer", "RoughnessBuffer.GBufferFramebuffer");

        //    this->AppendRenderPass(NormalPass);
        //}

        this->Activate();
    }
}
