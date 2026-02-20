#ifndef _SE_EVENTPROCESSER_H_
#define _SE_EVENTPROCESSER_H_
#include "EventDispatcher.h"

namespace SE
{
	class FWindowResizeEvent;

	class FEventProcesser
	{
	public:
		FEventProcesser() = default;
		FEventProcesser(const FEventProcesser&) = default;
		~FEventProcesser() = default;

		template<typename T>
		void OnEvent(const std::function<void(const T&)>& function);
	};

	template<typename T>
	inline void FEventProcesser::OnEvent(const std::function<void(const T&)>& function)
	{
		FEventDispatcher::Instance->RegisterEventProcesser(this, function);
	}

	template void FEventProcesser::OnEvent<FWindowResizeEvent>(const std::function<void(const FWindowResizeEvent&)>& function);
}

#endif