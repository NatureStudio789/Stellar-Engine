#include <Core.h>
#include "../Event/EventDispatcher.h"
#include "../Event/ApplicationEvent.h"
#include "Window.h"

namespace SE
{
	FWindow::FWindow() : SAddressable()
	{
		this->WindowAttribution = {};
		this->WindowHandle = null;
	}

	FWindow::FWindow(const Attribution& attribution)
	{
		this->Initialize(attribution);
	}

	FWindow::FWindow(const FWindow& other) : SAddressable(other)
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
			SMessageHandler::Instance->SetFatal("Application", "Failed to create window!");
		}

		this->WindowHandle = std::make_shared<Handle>(_WindowHandle);

		glfwSetWindowSizeCallback(this->WindowHandle->Instance, [](GLFWwindow* window, int width, int height)
			{
				FWindowResizeEvent event;
				event.WindowHandle = window;
				event.ResizeWidth = width;
				event.ResizeHeight = height;
				FEventDispatcher::Instance->DispatchEvent(event);
			});

		glfwSetWindowFocusCallback(this->WindowHandle->Instance, [](GLFWwindow* window, int focus)
			{
				if (focus == 1)
				{
					FWindowFocusEvent event;
					event.WindowHandle = window;
					FEventDispatcher::Instance->DispatchEvent(event);
				}
				else if (focus == 0)
				{
					FWindowLostFocusEvent event;
					event.WindowHandle = window;
					FEventDispatcher::Instance->DispatchEvent(event);
				}
			});

		this->WindowEventProcesser = std::make_shared<FEventProcesser>();
		this->WindowEventProcesser->OnEvent<FWindowMoveEvent>([this](const FWindowMoveEvent& event)
			{
				if (this->WindowHandle->Instance == event.WindowHandle)
				{
					this->WindowAttribution.WindowPosition.x = event.WindowPositionX;
					this->WindowAttribution.WindowPosition.y = event.WindowPositionY;
				}
			});

		this->WindowEventProcesser->OnEvent<FWindowResizeEvent>([this](const FWindowResizeEvent& event)
			{
				if (this->WindowHandle->Instance == event.WindowHandle)
				{
					this->WindowAttribution.WindowSize.x = event.ResizeWidth;
					this->WindowAttribution.WindowSize.y = event.ResizeHeight;
				}
			});

		glfwSetTitlebarHitTestCallback(this->WindowHandle->Instance, [](GLFWwindow* window, int x, int y, int* hit)
			{
				*hit = true;
			});

		this->Activate();
	}

	void FWindow::Display()
	{
		glfwShowWindow(this->WindowHandle->Instance);
		glfwSetWindowPos(this->WindowHandle->Instance, this->WindowAttribution.WindowPosition.x, this->WindowAttribution.WindowPosition.y);

		glfwSetWindowPosCallback(this->WindowHandle->Instance, [](GLFWwindow* window, int x, int y)
			{
				FWindowMoveEvent event;
				event.WindowHandle = window;
				event.WindowPositionX = x;
				event.WindowPositionY = y;
				FEventDispatcher::Instance->DispatchEvent(event);
			});

		this->WindowEventProcesser->OnEvent<FWindowMoveEvent>([this](const FWindowMoveEvent& event)
			{
				if (this->WindowHandle->Instance == event.WindowHandle)
				{
					this->WindowAttribution.WindowPosition.x = event.WindowPositionX;
					this->WindowAttribution.WindowPosition.y = event.WindowPositionY;
				}
			});
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
