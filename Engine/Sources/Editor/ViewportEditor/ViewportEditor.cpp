#include <Core.h>
#include "../../Runtime/Graphics/Framebuffer/Framebuffer.h"
#include "../../Runtime/Graphics/Texture/Texture.h"
#include "ViewportEditor.h"

namespace SE
{
	EViewportEditor::EViewportEditor() : EEditor()
	{

	}

	EViewportEditor::EViewportEditor(const std::string& name) : EEditor()
	{
		this->SetName(name);
	}

	EViewportEditor::EViewportEditor(const EViewportEditor& other) : EEditor(other)
	{

	}

	EViewportEditor::~EViewportEditor()
	{

	}

	void EViewportEditor::Initialize()
	{

	}

	void EViewportEditor::Render()
	{
		auto GBufferFramebuffer = SFramebufferRegistry::GetInstance("GBufferFramebuffer");
		auto CompositionFramebuffer = SFramebufferRegistry::GetInstance("FinalCompositionFramebuffer");

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("Albedo");

			ImGui::PopStyleVar();

			ImGui::Image((ImTextureID)GBufferFramebuffer->GetRTShaderResourceView(0)->GetGPUDescriptor().ptr,
				ImVec2((float)GBufferFramebuffer->GetWidth(), (float)GBufferFramebuffer->GetHeight()));

			ImGui::End();
		}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("Metallic");

			ImGui::PopStyleVar();

			ImGui::Image((ImTextureID)GBufferFramebuffer->GetRTShaderResourceView(1)->GetGPUDescriptor().ptr,
				ImVec2((float)GBufferFramebuffer->GetWidth(), (float)GBufferFramebuffer->GetHeight()));

			ImGui::End();
		}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("Roughness");

			ImGui::PopStyleVar();

			ImGui::Image((ImTextureID)GBufferFramebuffer->GetRTShaderResourceView(2)->GetGPUDescriptor().ptr,
				ImVec2((float)GBufferFramebuffer->GetWidth(), (float)GBufferFramebuffer->GetHeight()));

			ImGui::End();
		}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("Normal");

			ImGui::PopStyleVar();

			ImGui::Image((ImTextureID)GBufferFramebuffer->GetRTShaderResourceView(3)->GetGPUDescriptor().ptr,
				ImVec2((float)GBufferFramebuffer->GetWidth(), (float)GBufferFramebuffer->GetHeight()));

			ImGui::End();
		}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("Position");

			ImGui::PopStyleVar();

			ImGui::Image((ImTextureID)GBufferFramebuffer->GetRTShaderResourceView(4)->GetGPUDescriptor().ptr,
				ImVec2((float)GBufferFramebuffer->GetWidth(), (float)GBufferFramebuffer->GetHeight()));

			ImGui::End();
		}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("Viewport");

			glm::uvec2 ViewportSize = { (unsigned int)ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
			if (CompositionFramebuffer->GetSize() != ViewportSize)
			{
				CompositionFramebuffer->Resize(ViewportSize);
				GBufferFramebuffer->Resize(ViewportSize);
			}

			ImGui::PopStyleVar();

			ImGui::Image((ImTextureID)CompositionFramebuffer->GetRTShaderResourceView()->GetGPUDescriptor().ptr,
				ImVec2((float)CompositionFramebuffer->GetWidth(), (float)CompositionFramebuffer->GetHeight()));

			ImGui::End();
		}
	}
}
