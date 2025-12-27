#ifndef _SE_WINDOW_H_
#define _SE_WINDOW_H_
#include "../MonitorUtil/MonitorUtil.h"

namespace SE
{
	class FWindow
	{
	public:
		struct Attribution
		{
			Attribution()
			{
				this->WindowTitle = "Stellar Engine";
				this->WindowSize = {};
			}
			Attribution(const SString& title, const SVector2& size)
			{
				this->WindowTitle = title;
				this->WindowSize = size;
			}
			Attribution(const Attribution& ) = default;

			void SetDefaultSize()
			{
				this->WindowSize = FMonitorUtil::GetMonitorResolution() * 0.8f;
			}

			SString WindowTitle;
			SVector2 WindowSize;
		};

		struct Handle
		{
			Handle()
			{
				this->Instance = null;
			}
			Handle(GLFWwindow* instance)
			{
				this->Instance = instance;
			}
			Handle(const Handle& other)
			{
				this->Instance = other.Instance;
			}

#ifdef SE_PLATFORM_WINDOWS
			HWND GetWin32Handle()
			{
				return glfwGetWin32Window(this->Instance);
			}
#endif

			GLFWwindow* Instance;
		};

	public:
		FWindow();
		FWindow(const Attribution& attribution);
		FWindow(const FWindow& other);
		~FWindow();

		void Initialize(const Attribution& attribution);

		void Release();

		void ProcessMessage();

		SBool IsWindowRunning();

		const SString& GetWindowTitle() const noexcept;
		const SVector2& GetWindowSize() const noexcept;
		const Attribution& GetWindowAttribution() const noexcept;

		std::shared_ptr<Handle> GetWindowHandle();

	private:
		Attribution WindowAttribution;
		std::shared_ptr<Handle> WindowHandle;
	};
}

#endif