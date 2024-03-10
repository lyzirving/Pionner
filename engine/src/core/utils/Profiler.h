#ifndef __PIONNER_CORE_UTIL_STATISTICS_H__
#define __PIONNER_CORE_UTIL_STATISTICS_H__

#include "core/Base.h"

namespace pio
{
}

#ifdef PIO_PROFILER_ON

#define PROFILER_TIME pio::TimeUtil::CurrentTimeMs()
#define PROFILERD_DURATION(start, fmt, ...) pio::LogSystem::Log(pio::LogLevel::DEBUG, "STAT_DURATION| " fmt "| cost time[%lu]",\
							                                    ##__VA_ARGS__, pio::TimeUtil::CurrentTimeMs() - start)
#else 

#define PROFILER_TIME
#define PROFILERD_DURATION(start, fmt, ...)

#endif

#endif