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

		{
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
			
			if (!SWindowRegistry::GetMainInstance()->GetWindowMaximzied())
			{
				ImGuiWindow* window = ImGui::GetCurrentWindow();

				struct ImGuiResizeBorderDef
				{
					ImVec2 InnerDir;
					ImVec2 SegmentN1, SegmentN2;
					float  OuterAngle;
				};

				static const ImGuiResizeBorderDef resize_border_def[4] =
				{
					{ ImVec2(+1, 0), ImVec2(0, 1), ImVec2(0, 0), IM_PI * 1.00f },
					{ ImVec2(-1, 0), ImVec2(1, 0), ImVec2(1, 1), IM_PI * 0.00f },
					{ ImVec2(0, +1), ImVec2(0, 0), ImVec2(1, 0), IM_PI * 1.50f },
					{ ImVec2(0, -1), ImVec2(1, 1), ImVec2(0, 1), IM_PI * 0.50f } 
				};

				auto GetResizeBorderRect = [](ImGuiWindow* window, int border_n, float perp_padding, float thickness)
				{
					ImRect rect = window->Rect();
					if (thickness == 0.0f)
					{
						rect.Max.x -= 1;
						rect.Max.y -= 1;
					}
					if (border_n == ImGuiDir_Left) { return ImRect(rect.Min.x - thickness, rect.Min.y + perp_padding, rect.Min.x + thickness, rect.Max.y - perp_padding); }
					if (border_n == ImGuiDir_Right) { return ImRect(rect.Max.x - thickness, rect.Min.y + perp_padding, rect.Max.x + thickness, rect.Max.y - perp_padding); }
					if (border_n == ImGuiDir_Up) { return ImRect(rect.Min.x + perp_padding, rect.Min.y - thickness, rect.Max.x - perp_padding, rect.Min.y + thickness); }
					if (border_n == ImGuiDir_Down) { return ImRect(rect.Min.x + perp_padding, rect.Max.y - thickness, rect.Max.x - perp_padding, rect.Max.y + thickness); }
					IM_ASSERT(0);
					return ImRect();
				};


				ImGuiContext& g = *GImGui;
				float rounding = window->WindowRounding;
				float border_size = 1.0f;
				if (border_size > 0.0f && !(window->Flags & ImGuiWindowFlags_NoBackground))
					window->DrawList->AddRect(window->Pos, { window->Pos.x + window->Size.x,  window->Pos.y + window->Size.y }, 
						ImGui::GetColorU32(ImGuiCol_Border), rounding, 0, border_size);

				int border_held = window->ResizeBorderHeld;
				if (border_held != -1)
				{
					const ImGuiResizeBorderDef& def = resize_border_def[border_held];
					ImRect border_r = GetResizeBorderRect(window, border_held, rounding, 0.0f);
					ImVec2 p1 = ImLerp(border_r.Min, border_r.Max, def.SegmentN1);
					const float offsetX = def.InnerDir.x * rounding;
					const float offsetY = def.InnerDir.y * rounding;
					p1.x += 0.5f + offsetX;
					p1.y += 0.5f + offsetY;

					ImVec2 p2 = ImLerp(border_r.Min, border_r.Max, def.SegmentN2);
					p2.x += 0.5f + offsetX;
					p2.y += 0.5f + offsetY;

					window->DrawList->PathArcTo(p1, rounding, def.OuterAngle - IM_PI * 0.25f, def.OuterAngle);
					window->DrawList->PathArcTo(p2, rounding, def.OuterAngle, def.OuterAngle + IM_PI * 0.25f);
					window->DrawList->PathStroke(ImGui::GetColorU32(ImGuiCol_SeparatorActive), 0, ImMax(2.0f, border_size)); // Thicker than usual
				}
				if (g.Style.FrameBorderSize > 0 && !(window->Flags & ImGuiWindowFlags_NoTitleBar) && !window->DockIsActive)
				{
					float y = window->Pos.y + window->TitleBarHeight() - 1;
					window->DrawList->AddLine(ImVec2(window->Pos.x + border_size, y), ImVec2(window->Pos.x + window->Size.x - border_size, y), ImGui::GetColorU32(ImGuiCol_Border), g.Style.FrameBorderSize);
				}
			}

			ImGui::PopStyleColor();
		}

		const float titlebarHeight = 58.0f;
		float titlebarVerticalOffset = SWindowRegistry::GetMainInstance()->GetWindowMaximzied() ? -6.0f : 0.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

		{

			ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
			const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
			const ImVec2 titlebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
										 ImGui::GetCursorScreenPos().y + titlebarHeight };
			auto* bgDrawList = ImGui::GetBackgroundDrawList();
			auto* fgDrawList = ImGui::GetForegroundDrawList();
			bgDrawList->AddRectFilled(titlebarMin, titlebarMax, ImColor(0.5f, 0.08f, 0.5f));
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


		
		ImGui::End();


		ImGui::Render();
		SCommandListRegistry::GetCurrentInstance()->Open();

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

		SCommandListRegistry::GetCurrentInstance()->Close();
	}
}
