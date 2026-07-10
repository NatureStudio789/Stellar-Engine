#include <Core.h>
#include "Input.h"

namespace SE
{
	void FInput::InitializeInput(GLFWwindow* inputBindingWindow)
	{
		FKeyboard::InitializeKeyboard(inputBindingWindow);

		FMouse::InitializeMouse(inputBindingWindow);
	}

	void FInput::UpdateInput()
	{
		FKeyboard::UpdateKeyboard();

		FMouse::UpdateMouse();
	}
}
