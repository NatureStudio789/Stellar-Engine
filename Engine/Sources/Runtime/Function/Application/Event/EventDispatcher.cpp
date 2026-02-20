#include <Core.h>
#include "EventDispatcher.h"

namespace SE
{
	STELLAR_FINE_SINGLETON(FEventDispatcher, Instance);


	void FEventDispatcher::DestroyEventProcesser(FEventProcesser* processer)
	{
		for (auto& iterator : this->EventProcesserList)
		{
			iterator.second.erase(std::remove_if(iterator.second.begin(), iterator.second.end(),
				[processer](const FEventHandler& handler)
				{
					return (handler.EventProcesser && handler.EventProcesser == processer);
				}), iterator.second.end());
		}
	}

	void FEventDispatcher::DispatchEvent(FEvent& event)
	{
		if (this->EventProcesserList.find(event.GetEventType()) == this->EventProcesserList.end())
		{
			return;
		}

		auto& processers = this->EventProcesserList[event.GetEventType()];
		if (processers.empty())
		{
			return;
		}

		for (auto& processer : processers)
		{
			if (processer.EventProcesser)
			{
				processer.ProcessFunction(event);
			}
		}
	}
}
