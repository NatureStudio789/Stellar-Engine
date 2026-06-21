#include <Core.h>

#include "../Framebuffer/Framebuffer.h"

#include "RenderPass/ClearPass.h"

#include "RenderPass/DeferredRendering/AlbedoPass.h"
#include "RenderPass/DeferredRendering/MetallicPass.h"
#include "RenderPass/DeferredRendering/RoughnessPass.h"
#include "RenderPass/DeferredRendering/NormalPass.h"
#include "RenderPass/DeferredRendering/PositionPass.h"
#include "RenderPass/DeferredRendering/CompositionPass.h"

#include "DeferredRenderer.h"

namespace SE
{
    GDeferredRenderer::GDeferredRenderer(const std::string& name) : GRenderer(name)
    {
        //The Framebuffer initial size is temperory. It will change according to graphics configuration later.
        this->GBufferFramebuffer = GFramebuffer::Create(/*will be changed -> */glm::uvec2{ 2560, 1440 }, 5);
        this->GBufferFramebuffer->SetName("GBufferFramebuffer");
        SFramebufferRegistry::Register(this->GBufferFramebuffer);

        this->AddGlobalOutflow(GOutflow::Create("GBufferFramebuffer", this->GBufferFramebuffer->GetResourcePackage()));

        this->FinalCompositionFramebuffer = GFramebuffer::Create(/*will be changed -> */glm::uvec2{2560, 1440});
        this->FinalCompositionFramebuffer->SetName("FinalCompositionFramebuffer");
        SFramebufferRegistry::Register(this->FinalCompositionFramebuffer);

        this->AddGlobalOutflow(GOutflow::Create("CompositionFramebuffer", this->FinalCompositionFramebuffer->GetResourcePackage()));

        {
            auto GBufferClearingPass = std::make_shared<GClearPass>("GBufferClearingPass", std::vector<unsigned int>{ 0, 1, 2, 3, 4 });
            GBufferClearingPass->SetLinkage("ClearingFramebuffer", "$.GBufferFramebuffer");

            this->AppendRenderPass(GBufferClearingPass);
        }

        {
            auto CompositionFrameClearingPass = std::make_shared<GClearPass>("CompositionBufferClearingPass");
            CompositionFrameClearingPass->SetLinkage("ClearingFramebuffer", "$.CompositionFramebuffer");

            this->AppendRenderPass(CompositionFrameClearingPass);
        }

        {
            auto AlbedoPass = std::make_shared<GAlbedoPass>("AlbedoBuffer");
            AlbedoPass->SetLinkage("GBufferFramebuffer", "GBufferClearingPass.ClearingFramebuffer");

            this->AppendRenderPass(AlbedoPass);
        }

        {
            auto MetallicPass = std::make_shared<GMetallicPass>("MetallicBuffer");
            MetallicPass->SetLinkage("GBufferFramebuffer", "AlbedoBuffer.GBufferFramebuffer");

            this->AppendRenderPass(MetallicPass);
        }

        {
            auto RoughnessPass = std::make_shared<GRoughnessPass>("RoughnessBuffer");
            RoughnessPass->SetLinkage("GBufferFramebuffer", "MetallicBuffer.GBufferFramebuffer");

            this->AppendRenderPass(RoughnessPass);
        }

        {
            auto NormalPass = std::make_shared<GNormalPass>("NormalBuffer");
            NormalPass->SetLinkage("GBufferFramebuffer", "RoughnessBuffer.GBufferFramebuffer");

            this->AppendRenderPass(NormalPass);
        }

        {
            auto PositionPass = std::make_shared<GPositionPass>("PositionBuffer");
            PositionPass->SetLinkage("GBufferFramebuffer", "NormalBuffer.GBufferFramebuffer");

            this->AppendRenderPass(PositionPass);
        }

        {
            auto CompositionPass = std::make_shared<GCompositionPass>("CompositionPass");
            CompositionPass->SetLinkage("CompositionFramebuffer", "CompositionBufferClearingPass.ClearingFramebuffer");
            CompositionPass->SetLinkage("GBuffer", "PositionBuffer.GBufferFramebuffer");

            this->AppendRenderPass(CompositionPass);
        }

        this->Activate();
    }
}
