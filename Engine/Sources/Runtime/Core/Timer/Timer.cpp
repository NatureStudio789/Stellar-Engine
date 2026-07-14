#include <Core.h>
#include "Timer.h"

namespace SE
{
	STimer::STimer()
	{
		this->StartTimePoint = std::chrono::high_resolution_clock::now();
		this->StopTimePoint = std::chrono::high_resolution_clock::now();

		this->IsRunning = false;
	}

	STimer::STimer(const STimer& other)
	{
		this->IsRunning = other.IsRunning;
		this->StartTimePoint = other.StartTimePoint;
		this->StopTimePoint = other.StopTimePoint;
	}

	STimer::~STimer()
	{
		this->IsRunning = false;
	}

	void STimer::Restart()
	{
		this->IsRunning = true;
		this->StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	void STimer::Start()
	{
		this->IsRunning = true;

		this->StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	void STimer::Stop()
	{
		this->StopTimePoint = std::chrono::high_resolution_clock::now();

		this->IsRunning = false;
	}

	float STimer::GetElapsedTime() const noexcept
	{
		if (this->IsRunning)
		{
			auto ElapsedTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - this->StartTimePoint);

			return ElapsedTime.count();
		}
		else
		{
			auto ElapsedTime = std::chrono::duration<float, std::milli>(this->StopTimePoint - this->StartTimePoint);

			return ElapsedTime.count();
		}
	}
}
