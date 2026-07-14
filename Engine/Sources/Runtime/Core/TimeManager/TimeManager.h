#ifndef _SE_TIMEMANAGER_H_
#define _SE_TIMEMANAGER_H_
#include "../Timer/Timer.h"

namespace SE
{
	class STimeManager
	{
	public:
		static void InitializeTimeManager();

		static void UpdateTimeManager();

		static float GetFrameDeltaTime();
		static int GetFramePerSecond();

	private:
		static std::shared_ptr<STimer> MainTimer;

		static float FrameDeltaTime;
		static int FramePerSecond;
	};
}

#endif