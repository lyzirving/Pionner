#ifndef __PIONNER_BASE_UTILS_TIME_H__
#define __PIONNER_BASE_UTILS_TIME_H__

#include "base/defs/FunDef.h"

namespace pio
{
	class Tick
	{
	public:
		void tick();

		uint64_t Delta{ 0 };
	private:
		uint64_t m_lastTick{ 0 };
	};

	class Time
	{
	public:
		static uint64_t CurrentTimeMs();
		static uint64_t CurrentTimeMicro();
		static void RecordTime();

		static uint64_t k_LifeTime;
		static uint64_t k_StartTime;
		static uint64_t k_FrameTime;
		static Tick k_Tick;
	};
}

#endif