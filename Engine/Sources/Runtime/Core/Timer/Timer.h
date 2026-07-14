#ifndef _SE_TIMER_H_
#define _SE_TIMER_H_

namespace SE
{
	class STimer
	{
	public:
		STimer();
		STimer(const STimer& other);
		~STimer();

		void Restart();
		
		void Start();
		void Stop();

		float GetElapsedTime() const noexcept;

	private:
		bool IsRunning;

		std::chrono::time_point<std::chrono::steady_clock> StartTimePoint;
		std::chrono::time_point<std::chrono::steady_clock> StopTimePoint;
	};
}

#endif