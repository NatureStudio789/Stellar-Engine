#ifndef _SE_EVENTDISPATCHER_H_
#define _SE_EVENTDISPATCHER_H_
#include "ApplicationEvent.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"

namespace SE
{
	class FEventProcesser;
	using FEventProcessFunction = std::function<void(FEvent& e)>;

	struct FEventHandler
	{
		FEventProcesser* EventProcesser;
		FEventProcessFunction ProcessFunction;
	};

	class FEventDispatcher
	{
	public:
		FEventDispatcher() = default;
		FEventDispatcher(const FEventDispatcher&) = delete;
		~FEventDispatcher() = default;

		template<typename T>
		void RegisterEventProcesser(FEventProcesser* processer, const std::function<void(const T&)>& function);
		void DestroyEventProcesser(FEventProcesser* processer);
		void DispatchEvent(FEvent& event);

		STELLAR_MAKE_SINGLETON(FEventDispatcher, Instance);
	
	private:
		std::unordered_map<FEvent::Category, std::vector<FEventHandler>> EventProcesserList;
	};

	template<typename T>
	inline void FEventDispatcher::RegisterEventProcesser(
		FEventProcesser* processer, const std::function<void(const T&)>& function)
	{

		auto EventFunction = [function](const FEvent& e)
		{
			const T& event = (const T&)e;

			return	function(event);
		};

		FEventHandler handler =
		{
			.EventProcesser = processer,
			.ProcessFunction = EventFunction
		};

		this->EventProcesserList[T::GetStaticEventType()].emplace_back(handler);
	}
}

#endif