#include "Time.h"

#include <chrono>
#include <ctime>

namespace pio
{	
	uint64_t Time::k_LifeTime = 0;
	uint64_t Time::k_StartTime = 0;
	uint64_t Time::k_FrameTime = 0;
	Tick Time::k_Tick{};

	void Tick::tick()
	{
		if (m_lastTick == 0)
		{
			m_lastTick = Time::CurrentTimeMs();
		}
		else
		{
			uint64_t cur = Time::CurrentTimeMs();
			Delta = cur - m_lastTick;
			m_lastTick = cur;
		}
	}

	uint64_t Time::CurrentTimeMs()
	{
		auto now = std::chrono::steady_clock::now();
		auto timePt = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
		return timePt.count();
	}

	uint64_t Time::CurrentTimeMicro()
	{
		auto now = std::chrono::steady_clock::now();
		auto timePt = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
		return timePt.count();
	}

	void Time::RecordTime()
	{
		if (k_StartTime == 0)
		{
			k_StartTime = CurrentTimeMs();
		}
		else
		{
			k_LifeTime = CurrentTimeMs() - k_StartTime;
		}
	}
}