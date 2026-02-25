#include <Core.h>
#include "Pass/EditorRenderPass.h"
#include "EditorRenderer.h"

namespace SE
{
	EEditorRenderer::EEditorRenderer(const std::string& name)
	{
		this->SetName(name);

		this->EditorMainFramebuffer = GFramebuffer::Create(SGraphicsContextRegistry::GetMainInstance()->GetSwapChain());
		this->EditorMainFramebuffer->SetName("EditorMainFramebuffer");
		SFramebufferRegistry::Register(this->EditorMainFramebuffer);

		this->EditorMainFramebufferPackage.Initialize(this->EditorMainFramebuffer->GetIdentifier(), GResourcePackage::SE_RESOURCE_FRAMEBUFFER);
		this->AddGlobalOutflow(GOutflow::Create("EditorRendererFramebuffer", this->EditorMainFramebufferPackage));

		{
			auto EditorRenderPass = std::make_shared<EEditorRenderPass>("EditorRender");
			EditorRenderPass->SetLinkage("EditorFramebuffer", "$.EditorRendererFramebuffer");

			this->AppendRenderPass(EditorRenderPass);
		}

		this->Activate();
	}
}
