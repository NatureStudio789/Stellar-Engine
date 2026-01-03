#include "Input.h"

namespace SE
{
	void FInput::InitializeInput(GLFWwindow* inputBindingWindow)
	{
		FKeyboard::InitializeKeyboard(inputBindingWindow);
	}

	void FInput::UpdateInput()
	{
		FKeyboard::UpdateKeyboard();
	}
}
