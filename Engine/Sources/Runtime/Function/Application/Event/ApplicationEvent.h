#ifndef _SE_APPLICATIONEVENT_H_
#define _SE_APPLICATIONEVENT_H_
#include "Event.h"

namespace SE
{
	class FWindowResizeEvent : public FEvent
	{
	public:
		FWindowResizeEvent()
		{
			this->WindowHandle = null;
			this->ResizeWidth = 0;
			this->ResizeHeight = 0;
		}
		FWindowResizeEvent(GLFWwindow* windowHandle, int width, int height)
		{
			this->WindowHandle = windowHandle;
			this->ResizeWidth = width;
			this->ResizeHeight = height;
		}
		FWindowResizeEvent(const FWindowResizeEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
			this->ResizeWidth = other.ResizeWidth;
			this->ResizeHeight = other.ResizeHeight;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_WINDOW_RESIZE)

		GLFWwindow* WindowHandle;
		int ResizeWidth;
		int ResizeHeight;
	};

	class FWindowMoveEvent : public FEvent
	{
	public:
		FWindowMoveEvent()
		{
			this->WindowHandle = null;
			this->WindowPositionX = 0;
			this->WindowPositionY = 0;
		}
		FWindowMoveEvent(GLFWwindow* windowHandle, int x, int y)
		{
			this->WindowHandle = windowHandle;
			this->WindowPositionX = x;
			this->WindowPositionY = y;
		}
		FWindowMoveEvent(const FWindowMoveEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
			this->WindowPositionX = other.WindowPositionX;
			this->WindowPositionY = other.WindowPositionY;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_WINDOW_MOVE)

		GLFWwindow* WindowHandle;
		int WindowPositionX;
		int WindowPositionY;
	};

	class FWindowFocusEvent : public FEvent
	{
	public:
		FWindowFocusEvent()
		{
			this->WindowHandle = null;
		}
		FWindowFocusEvent(GLFWwindow* windowHandle)
		{
			this->WindowHandle = windowHandle;
		}
		FWindowFocusEvent(const FWindowFocusEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_WINDOW_FOCUS)

		GLFWwindow* WindowHandle;
	};

	class FWindowLostFocusEvent : public FEvent
	{
	public:
		FWindowLostFocusEvent()
		{
			this->WindowHandle = null;
		}
		FWindowLostFocusEvent(GLFWwindow* windowHandle)
		{
			this->WindowHandle = windowHandle;
		}
		FWindowLostFocusEvent(const FWindowLostFocusEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_WINDOW_LOSTFOCUS)

		GLFWwindow* WindowHandle;
	};

	class FWindowCloseEvent : public FEvent
	{
	public:
		FWindowCloseEvent()
		{
			this->WindowHandle = null;
		}
		FWindowCloseEvent(GLFWwindow* windowHandle)
		{
			this->WindowHandle = windowHandle;
		}
		FWindowCloseEvent(const FWindowCloseEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_WINDOW_CLOSE)

		GLFWwindow* WindowHandle;
	};
}

#endif