#pragma once

#include "base/core/CoreDef.h"

namespace pio
{
	class Time
	{
	public:
		static uint64_t CurrentTimeMs();
		static uint64_t CurrentTimeMicro();		
		static void Tick();

		static uint64_t DeltaTimeMs() { return k_Delta; }
		static float Fps() { return k_FPSCounter.m_Fps; }

		static uint64_t k_LifeTime;
		static uint64_t k_StartTime;		
		static uint64_t k_LastTick;
		static uint64_t k_Delta;

	private:
		class FpsCounter
		{		
		public:
			FpsCounter(uint32_t roomSpeed, uint32_t numSeconds);
			~FpsCounter() = default;

			void CalcAverageFps(uint64_t deltaMs);

			std::vector<float> m_FpsArray;						
			uint32_t m_FramePassed{ 0 };
			float m_MovingSum{ 0.f };
			float m_Fps{ 0.f };
		};

		static FpsCounter k_FPSCounter;
	};
}