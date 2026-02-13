#ifndef _SE_WINDOW_H_
#define _SE_WINDOW_H_
#include "../MonitorUtil/MonitorUtil.h"
#include "../../../Core/Creatable/Creatable.h"
#include "../../../Core/Registry/Registry.h"
#include "../../../Core/MessageHandler/MessageHandler.h"

namespace SE
{
	class FWindow : public SCreatable<FWindow>
	{
	public:
		enum Style
		{
			SE_WINDOW_DEFAULT,
			SE_WINDOW_MODERN,
			SE_WINDOW_FULLSCREEN,
		};

		struct Attribution
		{
			Attribution()
			{
				this->WindowStyle = SE_WINDOW_DEFAULT;
				this->WindowTitle = "Stellar Engine";
			}
			Attribution(const Style& style, const std::string& title)
			{
				this->WindowStyle = style;
				this->WindowTitle = title;
			}
			Attribution(const Attribution& ) = default;

			const glm::uvec2& GetSize() const noexcept
			{
				return this->WindowSize;
			}

			const glm::uvec2& GetPosition() const noexcept
			{
				return this->WindowPosition;
			}

			const int& GetWidth() const noexcept
			{
				return this->WindowSize.x;
			}
			
			const int& GetHeight() const noexcept
			{
				return this->WindowSize.y;
			}

			Style WindowStyle;
			std::string WindowTitle;

		private:
			void InitializeTransform()
			{
				const auto& ScreenResolution = FMonitorUtil::GetMonitorResolution();

				switch (this->WindowStyle)
				{
					case SE_WINDOW_DEFAULT:
					{
						this->WindowSize = ScreenResolution / (unsigned int)5 * (unsigned int)3;
						break;
					}

					case SE_WINDOW_MODERN:
					{
						this->WindowSize = ScreenResolution / (unsigned int)10 * (unsigned int)7;
						break;
					}

					case SE_WINDOW_FULLSCREEN:
					{
						this->WindowSize = ScreenResolution;
						break;
					}
				};

				this->WindowPosition = { (ScreenResolution.x - this->WindowSize.x) / 2, (ScreenResolution.y - this->WindowSize.y) / 2 };
			}

			glm::uvec2 WindowPosition;
			glm::uvec2 WindowSize;

			friend class FWindow;
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
		void Display();
		void Release();

		void ProcessMessage();

		bool IsWindowRunning();

		const std::string& GetWindowTitle() const noexcept;
		const glm::uvec2& GetWindowSize() const noexcept;
		const glm::uvec2& GetWindowPosition() const noexcept;
		const Attribution& GetWindowAttribution() const noexcept;

		std::shared_ptr<Handle> GetWindowHandle();

	private:
		Attribution WindowAttribution;
		std::shared_ptr<Handle> WindowHandle;
	};

	STELLAR_MAKE_DEFAULT_REGISTRY(FWindow, WindowRegistry);
}

#endif