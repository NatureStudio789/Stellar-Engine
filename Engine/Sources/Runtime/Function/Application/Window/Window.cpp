#include <Core.h>
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
		this->WindowAttribution.InitializeTransform();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef SE_PLATFORM_MACOS
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		switch (this->WindowAttribution.WindowStyle)
		{
			case SE_WINDOW_DEFAULT:
			{
				glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
				break;
			}

			case SE_WINDOW_MODERN:
			{
				glfwWindowHint(GLFW_TITLEBAR, GLFW_FALSE);
				glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
				break;
			}
		}

		GLFWwindow* _WindowHandle = glfwCreateWindow(this->WindowAttribution.WindowSize.x,
			this->WindowAttribution.WindowSize.y, this->WindowAttribution.WindowTitle.c_str(), 
			this->WindowAttribution.WindowStyle == SE_WINDOW_FULLSCREEN ? glfwGetPrimaryMonitor() : null, null);
		if (!_WindowHandle)
		{
			// TO DO: Make the error system.
			return;
		}
		this->WindowHandle = std::make_shared<Handle>(_WindowHandle);
	}

	void FWindow::Display()
	{
		glfwShowWindow(this->WindowHandle->Instance);
		glfwSetWindowPos(this->WindowHandle->Instance, this->WindowAttribution.WindowPosition.x, this->WindowAttribution.WindowPosition.y);
	}

	void FWindow::Release()
	{
		glfwDestroyWindow(this->WindowHandle->Instance);
	}

	void FWindow::ProcessMessage()
	{
		glfwPollEvents();
	}

	bool FWindow::IsWindowRunning()
	{
		return !glfwWindowShouldClose(this->WindowHandle->Instance);
	}

	const std::string& FWindow::GetWindowTitle() const noexcept
	{
		return this->WindowAttribution.WindowTitle;
	}

	const glm::uvec2& FWindow::GetWindowSize() const noexcept
	{
		return this->WindowAttribution.WindowSize;
	}

	const glm::uvec2& FWindow::GetWindowPosition() const noexcept
	{
		return this->WindowAttribution.WindowPosition;
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
