#ifndef _SE_KEYBOARDEVENT_H_
#define _SE_KEYBOARDEVENT_H_
#include "Event.h"

namespace SE
{
	class FKeyPressEvent : public FEvent
	{
	public:
		FKeyPressEvent()
		{
			this->KeyCode = 0;
		}
		FKeyPressEvent(unsigned int keyCode)
		{
			this->KeyCode = keyCode;
		}
		FKeyPressEvent(const FKeyPressEvent& other)
		{
			this->KeyCode = other.KeyCode;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_KEY_PRESS)

		unsigned int KeyCode;
	};

	class FKeyReleaseEvent : public FEvent
	{
	public:
		FKeyReleaseEvent()
		{
			this->KeyCode = 0;
		}
		FKeyReleaseEvent(unsigned int keyCode)
		{
			this->KeyCode = keyCode;
		}
		FKeyReleaseEvent(const FKeyReleaseEvent& other)
		{
			this->KeyCode = other.KeyCode;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_KEY_RELEASE)

		unsigned int KeyCode;
	};

	class FCharEvent : public FEvent
	{
	public:
		FCharEvent()
		{
			this->CharCode = 0;
		}
		FCharEvent(unsigned char charCode)
		{
			this->CharCode = charCode;
		}
		FCharEvent(const FCharEvent& other)
		{
			this->CharCode = other.CharCode;
		}

		STELLAR_EVENT_TYPE(SE_EVENT_CHAR)

		unsigned char CharCode;
	};
}

#endif