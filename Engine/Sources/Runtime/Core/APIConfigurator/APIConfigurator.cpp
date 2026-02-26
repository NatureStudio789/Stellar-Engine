#include <Core.h>
#include "../../Function/Application/Window/Window.h"
#include "../../Graphics/Context/GraphicsContext.h"
#include "APIConfigurator.h"

namespace SE
{
	void SAPIConfigurator::InitializeAPI()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef SE_PLATFORM_MACOS
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(10.0f, 10.0f);
		style.FramePadding = ImVec2(8.0f, 6.0f);
		style.ItemSpacing = ImVec2(6.0f, 6.0f);
		style.ChildRounding = 6.0f;
		style.PopupRounding = 6.0f;
		style.FrameRounding = 6.0f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
	}

	void SAPIConfigurator::PostInitializeAPI()
	{
		ImGui_ImplGlfw_InitForOther(SWindowRegistry::GetMainInstance()->GetWindowHandle()->Instance, true);

		auto FontDescriptorHandle = SGraphicsContextRegistry::GetMainInstance()->GetSRVDescriptorHeap()->Allocate();
		ImGui_ImplDX12_Init(SGraphicsContextRegistry::GetMainInstance()->GetDevice()->GetInstance().Get(),
			SGraphicsContextRegistry::GetMainInstance()->GetSwapChain()->GetPresentBuffer()->Count,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			SGraphicsContextRegistry::GetMainInstance()->GetSRVDescriptorHeap()->GetInstance().Get(),
			FontDescriptorHandle->CPUHandle, FontDescriptorHandle->GPUHandle);
	}

	void SAPIConfigurator::ShutdownAPI()
	{
		glfwTerminate();
	}
}
