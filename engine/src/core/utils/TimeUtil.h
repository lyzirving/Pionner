#ifndef __PIONNER_CORE_UTILS_TIMEUTIL_H__
#define __PIONNER_CORE_UTILS_TIMEUTIL_H__

#include <cstdint>

namespace pio
{
	class TimeUtil
	{
	public:
		static uint64_t CurrentTimeMicro();
		static uint64_t CurrentTimeMs();

	private:
		TimeUtil() {};
		~TimeUtil() = default;
	};

	class Timestep
	{
	public:
		Timestep(uint64_t timeMs = 0) : m_timeMs(timeMs), m_tick(TimeUtil::CurrentTimeMs())
		{
		}

		inline float    getSeconds()      const { return float(m_timeMs) / 1000.f; }
		inline uint64_t getMilliseconds() const { return m_timeMs; }

		/*
		* Type transformation function: transform Timestep to float.
		* Syntax of type transformation function: operator type() const;
		* Three features of type transformation function are listed as bellow:
		* 1. define in source class;
		* 2. decorate by <operator>, function name must be target class
		* 3. no parameter and no return type in declaration, but in function body, we must have return value
		*/
		operator float() const { return getSeconds(); }
		operator uint64_t() const { return m_timeMs; }

		void tick()
		{
			uint64_t cur = TimeUtil::CurrentTimeMs();
			m_timeMs = cur - m_tick;
			m_tick = cur;
		}

	private:
		uint64_t m_timeMs;
		uint64_t m_tick;
	};
}

#endif