#include <Core.h>
#include "Keyboard.h"

namespace SE
{
	std::map<unsigned char, FKeyboard::KeyState> FKeyboard::KeyStateList;


	void FKeyboard::InitializeKeyboard(GLFWwindow* bindingWindow)
	{
		KeyStateList[SE_KEY_SPACE] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_APOSTROPHE] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_COMMA] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_MINUS] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_PERIOD] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_SLASH] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_0] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_1] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_2] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_3] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_4] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_5] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_6] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_7] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_8] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_9] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_SEMICOLON] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_EQUAL] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_A] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_B] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_C] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_D] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_E] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_G] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_H] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_I] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_J] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_K] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_L] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_M] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_N] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_O] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_P] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_Q] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_R] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_S] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_T] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_U] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_V] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_W] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_X] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_Y] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_Z] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_LEFT_BRACKET] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_BACKSLASH] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_RIGHT_BRACKET] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_GRAVE_ACCENT] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_WORLD_1] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_WORLD_2] = SE_KEY_RELEASE;
		
		KeyStateList[SE_KEY_ESCAPE] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_ENTER] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_TAB] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_BACKSPACE] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_INSERT] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_DELETE] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_RIGHT] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_LEFT] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_DOWN] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_UP] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_PAGE_UP] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_PAGE_DOWN] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_HOME] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_END] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_CAPS_LOCK] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_SCROLL_LOCK] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_NUM_LOCK] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_PRINT_SCREEN] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_PAUSE] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F1] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F2] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F3] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F4] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F5] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F6] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F7] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F8] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F9] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F10] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F11] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F12] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F13] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F14] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F15] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F16] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F17] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F18] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F19] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F20] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F21] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F22] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F23] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F24] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_F25] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_0] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_1] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_2] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_3] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_4] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_5] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_6] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_7] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_8] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_9] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_DECIMAL] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_DIVIDE] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_MULTIPLY] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_SUBTRACT] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_ADD] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_ENTER] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_KP_EQUAL] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_LEFT_SHIFT] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_LEFT_CONTROL] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_LEFT_ALT] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_LEFT_SUPER] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_RIGHT_SHIFT] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_RIGHT_CONTROL] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_RIGHT_ALT] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_RIGHT_SUPER] = SE_KEY_RELEASE;
		KeyStateList[SE_KEY_MENU] = SE_KEY_RELEASE;
		
		glfwSetKeyCallback(bindingWindow, [](GLFWwindow* window, int key, int scancode, int acton, int mods)
		{
			switch (acton)
			{
				case GLFW_RELEASE:
				{
					KeyStateList[(unsigned char)key] = SE_KEY_RELEASE;
					break;
				}

				case GLFW_PRESS:
				{
					if (KeyStateList[(unsigned char)key] == SE_KEY_PRESS || KeyStateList[(unsigned char)key] == SE_KEY_REPEAT)
					{
						KeyStateList[(unsigned char)key] = SE_KEY_REPEAT;
					}
					else
					{
						KeyStateList[(unsigned char)key] = SE_KEY_PRESS;
					}
					break;
				}

				case GLFW_REPEAT:
				{
					KeyStateList[(unsigned char)key] = SE_KEY_REPEAT;
					break;
				}
			}
		});
	}

	void FKeyboard::UpdateKeyboard()
	{

	}

	FKeyboard::KeyState FKeyboard::GetKeyState(unsigned char key)
	{
		return KeyStateList[key];
	}

	bool FKeyboard::GetKeyPress(unsigned char key, bool repeat)
	{
		if (repeat)
		{
			return KeyStateList[key] == SE_KEY_PRESS || KeyStateList[key] == SE_KEY_REPEAT;
		}
		else
		{
			return KeyStateList[key] == SE_KEY_PRESS;
		}
	}
}
