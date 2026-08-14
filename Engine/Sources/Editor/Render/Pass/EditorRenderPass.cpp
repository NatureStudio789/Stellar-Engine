#include <Core.h>
#include "../../Runtime/Graphics/Framebuffer/Framebuffer.h"
#include "../../Runtime/Graphics/Texture/Texture.h"
#include "../../../Runtime/Function/Input/Input.h"
#include "EditorRenderPass.h"

namespace SE
{
	EEditorRenderPass::EEditorRenderPass(const std::string& name) : GRenderPass(name)
	{
		this->AddInflow(GInflow::Create("EditorFramebuffer", this->FramebufferPackage));

		this->EngineIcon_Large = GTexture::Create("Engine/Assets/Textures/Stellar-Engine_Large.png");
		this->EngineIcon_Small = GTexture::Create("Engine/Assets/Textures/Stellar-Engine_Small.png");
	}

	void EEditorRenderPass::Execute()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, 
			SWindowRegistry::GetMainInstance()->GetWindowMaximzied() ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::PopStyleVar(2);

		const float titlebarHeight = 88.0f;
		float titlebarVerticalOffset = SWindowRegistry::GetMainInstance()->GetWindowMaximzied() ? -6.0f : 0.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

		{

			ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
			const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
			const ImVec2 titlebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
										 ImGui::GetCursorScreenPos().y + titlebarHeight };
			auto* bgDrawList = ImGui::GetBackgroundDrawList();
			auto* fgDrawList = ImGui::GetForegroundDrawList();
			bgDrawList->AddRectFilled(titlebarMin, titlebarMax, ImColor(0.5f, 0.08f, 0.8f));

			const int logoWidth = 48;
			const int logoHeight = 48;
			const ImVec2 logoOffset(windowPadding.x, windowPadding.y + titlebarVerticalOffset);
			const ImVec2 logoRectStart = { ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y };
			const ImVec2 logoRectMax = { logoRectStart.x + logoWidth, logoRectStart.y + logoHeight };
			fgDrawList->AddImage((ImTextureID)this->EngineIcon_Small->GetGPUDescriptor().ptr, logoRectStart, logoRectMax);
		}

		ImGui::BeginHorizontal("Titlebar", { ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing() });

		static float moveOffsetX;
		static float moveOffsetY;
		const float w = ImGui::GetContentRegionAvail().x;
		const float buttonsAreaWidth = 94;

		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight));

		SWindowRegistry::GetMainInstance()->SetTitleBarHovered(ImGui::IsItemHovered());

		{
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImVec2 textSize = ImGui::CalcTextSize("Stellar Engine");
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
			ImGui::Text("%s", "Stellar Engine");
			ImGui::SetCursorPos(currentCursorPos);
		}

		ImGui::EndHorizontal();

		ImGui::DockSpace(ImGui::GetID("MyDockspace"));


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Icon");

		ImGui::PopStyleVar();

		ImGui::Image((ImTextureID)this->EngineIcon_Large->GetGPUDescriptor().ptr, ImVec2(894.0f, 894.0f));

		ImGui::End();

		for (auto& [uuid, editor] : SEditorRegistry::GetInstanceList())
		{
			if (SEditorRegistry::GetEditorActive(uuid))
			{
				editor->Render();
			}
		}

		
		ImGui::End();


		ImGui::Render();

		this->GetFramebufferInstance(this->FramebufferPackage)->Begin();
		
		SGraphicsContextRegistry::GetMainInstance()->ApplyDescriptorHeaps();

		this->GetFramebufferInstance(this->FramebufferPackage)->Apply();
		this->GetFramebufferInstance(this->FramebufferPackage)->Clear({0.1f, 0.1f, 0.1f, 1.0f});

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(),
			SCommandListRegistry::GetCurrentInstance()->GetInstance().Get());

		this->GetFramebufferInstance(this->FramebufferPackage)->End();

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(null, (void*)SCommandListRegistry::GetCurrentInstance()->GetInstance().Get());
		}
	}
}
