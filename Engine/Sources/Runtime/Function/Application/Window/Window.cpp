#include "Window.h"

namespace SE
{
	FWindow::FWindow()
	{
		this->WindowAttribution = {};
		this->WindowHandle = null;
	}

	FWindow::FWindow(const Attribution& attribution)
	{
		this->Initialize(attribution);
	}

	FWindow::FWindow(const FWindow& other)
	{
		this->WindowAttribution = other.WindowAttribution;
		this->WindowHandle = other.WindowHandle;
	}

	FWindow::~FWindow()
	{
		this->Release();
	}

	void FWindow::Initialize(const Attribution& attribution)
	{
		this->WindowAttribution = attribution;

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
#ifdef SE_PLATFORM_MACOS
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		GLFWwindow* _WindowHandle = glfwCreateWindow((SInt)this->WindowAttribution.WindowSize.x,
			(SInt)this->WindowAttribution.WindowSize.y, this->WindowAttribution.WindowTitle.c_str(), null, null);
		if (!_WindowHandle)
		{
			// TO DO: Make the error system.
			return;
		}
		this->WindowHandle = std::make_shared<Handle>(_WindowHandle);

		glfwSetWindowPos(this->WindowHandle->Instance, (FMonitorUtil::GetMonitorResolutionX() - (SInt)this->WindowAttribution.WindowSize.x) / 2,
			(FMonitorUtil::GetMonitorResolutionY() - (SInt)this->WindowAttribution.WindowSize.y) / 2);
	}

	void FWindow::Release()
	{
		glfwDestroyWindow(this->WindowHandle->Instance);
	}

	void FWindow::ProcessMessage()
	{
		glfwPollEvents();
	}

	SBool FWindow::IsWindowRunning()
	{
		return !glfwWindowShouldClose(this->WindowHandle->Instance);
	}

	const SString& FWindow::GetWindowTitle() const noexcept
	{
		return this->WindowAttribution.WindowTitle;
	}

	const SVector2& FWindow::GetWindowSize() const noexcept
	{
		return this->WindowAttribution.WindowSize;
	}

	const FWindow::Attribution& FWindow::GetWindowAttribution() const noexcept
	{
		return this->WindowAttribution;
	}

	std::shared_ptr<FWindow::Handle> FWindow::GetWindowHandle()
	{
		return this->WindowHandle;
	}
}
