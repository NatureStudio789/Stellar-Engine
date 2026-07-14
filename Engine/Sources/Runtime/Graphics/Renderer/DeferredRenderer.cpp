#include <Core.h>

#include "../Framebuffer/Framebuffer.h"

#include "RenderPass/ClearPass.h"

#include "RenderPass/DeferredRendering/AlbedoPass.h"
#include "RenderPass/DeferredRendering/MetallicPass.h"
#include "RenderPass/DeferredRendering/RoughnessPass.h"
#include "RenderPass/DeferredRendering/NormalPass.h"
#include "RenderPass/DeferredRendering/PositionPass.h"
#include "RenderPass/DeferredRendering/CompositionPass.h"

#include "Lighting/PointLight/PointLightRegistry.h"
#include "Lighting/DirectionalLight/DirectionalLightRegistry.h"

#include "DeferredRenderer.h"

namespace SE
{
    GDeferredRenderer::GDeferredRenderer(const std::string& name) : GRenderer(name)
    {
        //The Framebuffer initial size is temperory. It will change according to graphics configuration later.
        std::vector<DXGI_FORMAT> FormatList =
        {
            DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT_R16G16B16A16_FLOAT,
            DXGI_FORMAT_R16G16B16A16_FLOAT,
        };
        this->GBufferFramebuffer = GFramebuffer::Create(/*will be changed -> */glm::uvec2{ 2560, 1440 }, 5, FormatList);
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
            this->CompositionPass = std::make_shared<GCompositionPass>("CompositionPass");
            this->CompositionPass->SetLinkage("CompositionFramebuffer", "CompositionBufferClearingPass.ClearingFramebuffer");
            this->CompositionPass->SetLinkage("GBuffer", "PositionBuffer.GBufferFramebuffer");

            this->AppendRenderPass(this->CompositionPass);
        }

        this->Activate();
    }

    void GDeferredRenderer::SetLightRegistry(std::shared_ptr<GPointLightRegistry> registry)
    {
        this->CompositionPass->AddApplicable(registry->LightCBuffer);
    }

    void GDeferredRenderer::SetLightRegistry(std::shared_ptr<GDirectionalLightRegistry> registry)
    {
        this->CompositionPass->AddApplicable(registry->LightCBuffer);
    }
}
