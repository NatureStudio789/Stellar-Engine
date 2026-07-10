#ifndef _SE_MOUSE_H_
#define _SE_MOUSE_H_

namespace SE
{
	class FMouse
	{
	public:
		enum Button
		{
			SE_LEFT_BUTTON,
			SE_RIGHT_BUTTON,
			SE_MIDDLE_BUTTON
		};

		enum ButtonState
		{
			SE_BUTTON_RELEASE,
			SE_BUTTON_PRESS,
		};

	public:
		static void InitializeMouse(GLFWwindow* bindingWindow);

		static void AddMovementCallback(std::function<void(const glm::ivec2&)> callback);

		static void UpdateMouse();

		static ButtonState GetButtonState(Button button);
		static bool GetButtonPress(Button button);

		static const glm::uvec2& GetCursorPosition();

	private:
		static std::map<Button, ButtonState> ButtonStateList;

		static glm::uvec2 CursorPosition;

		static std::queue<glm::ivec2> CursorMovementList;
		static std::vector<std::function<void(const glm::ivec2&)>> MovementCallbackList;

		friend class FWindow;
	};
}

#endif