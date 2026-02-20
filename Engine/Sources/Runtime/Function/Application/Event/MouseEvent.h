#ifndef _SE_MOUSEEVENT_H_
#define _SE_MOUSEEVENT_H_
#include "Event.h"

namespace SE
{
	enum FMouseButtonCategory
	{
		GE_MOUSEBUTTON_NONE,

		GE_MOUSEBUTTON_LEFT,
		GE_MOUSEBUTTON_MIDDLE,
		GE_MOUSEBUTTON_RIGHT,
	};

	class FMouseButtonClickEvent : public FEvent
	{
	public:
		FMouseButtonClickEvent()
		{
			this->MouseButton = GE_MOUSEBUTTON_NONE;
		}
		FMouseButtonClickEvent(const FMouseButtonCategory& button)
		{
			this->MouseButton = button;
		}
		FMouseButtonClickEvent(const FMouseButtonClickEvent& other)
		{
			this->MouseButton = other.MouseButton;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_MOUSE_BUTTONCLICK)

		FMouseButtonCategory MouseButton;
	};

	class FMouseButtonReleaseEvent : public FEvent
	{
	public:
		FMouseButtonReleaseEvent()
		{
			this->MouseButton = GE_MOUSEBUTTON_NONE;
		}
		FMouseButtonReleaseEvent(const FMouseButtonCategory& button)
		{
			this->MouseButton = button;
		}
		FMouseButtonReleaseEvent(const FMouseButtonReleaseEvent& other)
		{
			this->MouseButton = other.MouseButton;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_MOUSE_BUTTONRELEASE)

		FMouseButtonCategory MouseButton;
	};

	class FMouseMoveEvent : public FEvent
	{
	public:
		FMouseMoveEvent()
		{
			this->MousePositionX = 0;
			this->MousePositionY = 0;
		}
		FMouseMoveEvent(int x, int y)
		{
			this->MousePositionX = x;
			this->MousePositionY = y;
		}
		FMouseMoveEvent(const FMouseMoveEvent& other)
		{
			this->MousePositionX = other.MousePositionX;
			this->MousePositionY = other.MousePositionY;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_MOUSE_MOVE)

		int MousePositionX;
		int MousePositionY;
	};

	class FMouseRawMoveEvent : public FEvent
	{
	public:
		FMouseRawMoveEvent()
		{
			this->MouseMovementX = 0.0f;
			this->MouseMovementY = 0.0f;
		}
		FMouseRawMoveEvent(float movementX, float movementY)
		{
			this->MouseMovementX = movementX;
			this->MouseMovementY = movementY;
		}
		FMouseRawMoveEvent(const FMouseRawMoveEvent& other)
		{
			this->MouseMovementX = other.MouseMovementX;
			this->MouseMovementY = other.MouseMovementY;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_MOUSE_RAWMOVE)

		float MouseMovementX;
		float MouseMovementY;
	};
}

#endif