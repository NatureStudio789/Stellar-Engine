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

		this->AddGlobalOutflow(GOutflow::Create("EditorRendererFramebuffer", 
			this->EditorMainFramebuffer->GetResourcePackage()));

		{
			auto EditorRenderPass = std::make_shared<EEditorRenderPass>("EditorRender");
			EditorRenderPass->SetLinkage("EditorFramebuffer", "$.EditorRendererFramebuffer");

			this->AppendRenderPass(EditorRenderPass);
		}

		this->EventProcesser = std::make_shared<FEventProcesser>();
		this->EventProcesser->OnEvent<FWindowResizeEvent>([this](const FWindowResizeEvent& event)
			{
				if (event.WindowHandle == SWindowRegistry::GetMainInstance()->GetWindowHandle()->Instance)
				{
					this->EditorMainFramebuffer->Resize({ event.ResizeWidth, event.ResizeHeight });
				}
			});

		this->Activate();
	}
}
