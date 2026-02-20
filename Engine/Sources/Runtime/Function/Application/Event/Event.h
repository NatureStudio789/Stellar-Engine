#ifndef _SE_EVENT_H_
#define _SE_EVENT_H_
#include <string>

namespace SE
{
#define STELLAR_EVENT_TYPE(type) static Category GetStaticEventType() { return Category::##type; }\
									Category GetEventType() override { return GetStaticEventType(); }\
									virtual const std::string GetEventName() const override { return #type; }

	class FEvent
	{
	public:
		enum Category
		{
			SE_EVENT_NONE,
			
			SE_EVENT_MODE_SWITCH,
			
			SE_EVENT_WINDOW_RESIZE,
			SE_EVENT_WINDOW_MOVE,
			SE_EVENT_WINDOW_FOCUS,
			SE_EVENT_WINDOW_LOSTFOCUS,
			SE_EVENT_WINDOW_CLOSE,
			
			SE_EVENT_KEY_PRESS,
			SE_EVENT_KEY_RELEASE,
			SE_EVENT_CHAR,
			
			SE_EVENT_MOUSE_BUTTONCLICK,
			SE_EVENT_MOUSE_BUTTONRELEASE,
			SE_EVENT_MOUSE_MOVE,
			SE_EVENT_MOUSE_RAWMOVE,
			SE_EVENT_MOUSE_SCROLL,
		};

	public:
		virtual Category GetEventType() = 0;
		virtual const std::string GetEventName() const = 0;

	protected:
	};
}

#endif