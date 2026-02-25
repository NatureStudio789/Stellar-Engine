#include <Core.h>
#include "../../Runtime/Graphics/Framebuffer/Framebuffer.h"
#include "EditorRenderPass.h"

namespace SE
{
	EEditorRenderPass::EEditorRenderPass(const std::string& name) : GRenderPass(name)
	{
		this->AddInflow(GInflow::Create("EditorFramebuffer", this->FramebufferPackage));
	}

	void EEditorRenderPass::Execute()
	{
		this->ActivateCommandList();

		ImGui_ImplDX12_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		SCommandListRegistry::GetCurrentInstance()->Open();
		SFramebufferRegistry::GetInstance(this->FramebufferPackage.GetResourceIdentifier().GetUUID())->Begin();
		
		SGraphicsContextRegistry::GetMainInstance()->ApplyDescriptorHeaps();
		SFramebufferRegistry::GetInstance(this->FramebufferPackage.GetResourceIdentifier().GetUUID())->Apply();
		SFramebufferRegistry::GetInstance(this->FramebufferPackage.GetResourceIdentifier().GetUUID())->Clear({0.1f, 0.1f, 0.1f, 1.0f});
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(),
			SCommandListRegistry::GetCurrentInstance()->GetInstance().Get());

		SFramebufferRegistry::GetInstance(this->FramebufferPackage.GetResourceIdentifier().GetUUID())->End();

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(null, (void*)SCommandListRegistry::GetCurrentInstance()->GetInstance().Get());
		}
		SCommandListRegistry::GetCurrentInstance()->Close();
	}
}
