#include <Core.h>
#include "TimeManager.h"

namespace SE
{
	std::shared_ptr<STimer> STimeManager::MainTimer;

	float STimeManager::FrameDeltaTime;
	int STimeManager::FramePerSecond = 0;


	void STimeManager::InitializeTimeManager()
	{
		MainTimer = std::make_shared<STimer>();
		MainTimer->Start();
	}

	void STimeManager::UpdateTimeManager()
	{
		FrameDeltaTime = MainTimer->GetElapsedTime();
		MainTimer->Restart();

		static float TimeStep = 0.0f;
		TimeStep += FrameDeltaTime;
		static int CurrentFrameCount = 0;
		CurrentFrameCount++;

		if (TimeStep >= 1000.0f)
		{
			TimeStep = 0.0f;

			FramePerSecond = CurrentFrameCount;
			CurrentFrameCount = 0;
		}
	}

	float STimeManager::GetFrameDeltaTime()
	{
		return FrameDeltaTime;
	}

	int STimeManager::GetFramePerSecond()
	{
		return FramePerSecond;
	}
}
