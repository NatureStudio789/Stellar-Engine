#include <Core.h>
#include "../../../Core/MessageHandler/MessageHandler.h"
#include "Mouse.h"

namespace SE
{
	std::map<FMouse::Button, FMouse::ButtonState> FMouse::ButtonStateList;

	glm::uvec2 FMouse::CursorPosition;

	std::queue<glm::ivec2> FMouse::CursorMovementList;
	std::vector<std::function<void(const glm::ivec2&)>> FMouse::MovementCallbackList;


	void FMouse::InitializeMouse(GLFWwindow* bindingWindow)
	{
		ButtonStateList[Button::SE_LEFT_BUTTON] = SE_BUTTON_RELEASE;
		ButtonStateList[Button::SE_RIGHT_BUTTON] = SE_BUTTON_RELEASE;
		ButtonStateList[Button::SE_MIDDLE_BUTTON] = SE_BUTTON_RELEASE;

		glfwSetMouseButtonCallback(bindingWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			switch (button)
			{
				case GLFW_MOUSE_BUTTON_LEFT:
				{
					if (action == GLFW_RELEASE)
					{
						ButtonStateList[Button::SE_LEFT_BUTTON] = SE_BUTTON_RELEASE;
					}
					else if (action == GLFW_PRESS)
					{
						ButtonStateList[Button::SE_LEFT_BUTTON] = SE_BUTTON_PRESS;
					}
					break;
				}

				case GLFW_MOUSE_BUTTON_RIGHT:
				{
					if (action == GLFW_RELEASE)
					{
						ButtonStateList[Button::SE_RIGHT_BUTTON] = SE_BUTTON_RELEASE;
					}
					else if (action == GLFW_PRESS)
					{
						ButtonStateList[Button::SE_RIGHT_BUTTON] = SE_BUTTON_PRESS;
					}
					break;
				}

				case GLFW_MOUSE_BUTTON_MIDDLE:
				{
					if (action == GLFW_RELEASE)
					{
						ButtonStateList[Button::SE_MIDDLE_BUTTON] = SE_BUTTON_RELEASE;
					}
					else if (action == GLFW_PRESS)
					{
						ButtonStateList[Button::SE_MIDDLE_BUTTON] = SE_BUTTON_PRESS;
					}
					break;
				}
			}
		});

		glfwSetCursorPosCallback(bindingWindow, [](GLFWwindow* window, double mposX, double mposY)
		{
			CursorPosition = { (unsigned int)mposX, (unsigned int)mposY };
		});

		RAWINPUTDEVICE RawInputDevice = { 0 };
		RawInputDevice.usUsagePage = 0x01;
		RawInputDevice.usUsage = 0x02;
		RawInputDevice.dwFlags = 0;
		RawInputDevice.hwndTarget = null;

		if (!RegisterRawInputDevices(&RawInputDevice, 1, sizeof(RAWINPUTDEVICE)))
		{
			SMessageHandler::Instance->SetFatal("Input", "Failed to register raw input device!");
		}
	}

	void FMouse::AddMovementCallback(std::function<void(const glm::ivec2&)> callback)
	{
		MovementCallbackList.push_back(callback);
	}

	void FMouse::UpdateMouse()
	{
		if (CursorMovementList.empty())
		{
			return;
		}

		glm::ivec2 totalMovement = { 0, 0 };
		while (!CursorMovementList.empty())
		{
			totalMovement += CursorMovementList.front();
			CursorMovementList.pop();
		}

		for (const auto& callback : MovementCallbackList)
		{
			callback(totalMovement);
		}
	}

	FMouse::ButtonState FMouse::GetButtonState(Button button)
	{
		return ButtonStateList[button];
	}

	bool FMouse::GetButtonPress(Button button)
	{
		return ButtonStateList[button] == SE_BUTTON_PRESS;
	}

	const glm::uvec2& FMouse::GetCursorPosition()
	{
		return CursorPosition;
	}
}
