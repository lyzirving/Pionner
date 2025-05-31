#include "Time.h"
#include "base/math/MathDef.h"

#include <chrono>
#include <ctime>

namespace pio
{	
	uint64_t Time::k_LifeTime = 0;
	uint64_t Time::k_StartTime = 0;	
	uint64_t Time::k_LastTick = 0;
	uint64_t Time::k_Delta = 0;
	Time::FpsCounter Time::k_FPSCounter(60, 5);

	Time::FpsCounter::FpsCounter(uint32_t roomSpeed, uint32_t numSeconds)
	{
		m_FpsArray.resize(roomSpeed * numSeconds, 0);
	}

	void Time::FpsCounter::CalcAverageFps(uint64_t deltaMs)
	{				
		float divisor = float(deltaMs) / 1000.f;
		float realFps{ 0.f };
		if(!Math::IsZero(divisor))
		{
			realFps = 1.f / divisor;
		}

		uint32_t index = m_FramePassed;
		m_MovingSum -= m_FpsArray[index];
		m_FpsArray[index] = realFps;
		m_MovingSum += realFps;

		uint32_t arraySize = m_FpsArray.size();
		m_FramePassed = (m_FramePassed + 1) % arraySize;
		m_Fps = m_MovingSum / (float)arraySize;
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

	void Time::Tick()
	{
		if (k_StartTime == 0)
		{
			k_StartTime = CurrentTimeMs();
			k_LastTick = k_StartTime;
		}
		else
		{
			auto currentTime = CurrentTimeMs();
			k_LifeTime = currentTime - k_StartTime;
			k_Delta = currentTime - k_LastTick;
			k_LastTick = currentTime;
			k_FPSCounter.CalcAverageFps(k_Delta);
		}
	}
}