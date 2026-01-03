#ifndef _SE_INPUT_H_
#define _SE_INPUT_H_
#include "Keyboard/Keyboard.h"

namespace SE
{
	class FInput
	{
	public:
		static void InitializeInput(GLFWwindow* inputBindingWindow);

		static void UpdateInput();
	};
}

#endif