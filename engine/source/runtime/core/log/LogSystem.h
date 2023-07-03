#ifndef __LOG_SYSTEM_H__
#define __LOG_SYSTEM_H__

#include <memory>
#include <cstdint>

#ifdef LIB_TAB
#undef LIB_TAB
#endif
#define LIB_TAB "Pionner"

namespace spdlog
{
	class logger;
}

namespace Pionner
{
	enum LogLevel : uint8_t
	{
		DEBUG,
		INFO,
		WARN,
		ERR,
		FATAL
	};

	class LogSystem
	{
	public:
		static void initialize();
		static void shutdown();

		static void log(LogLevel level, const char *fmt, ...);

	private:
		LogSystem() {};
		~LogSystem() = default;

		static std::shared_ptr<spdlog::logger> g_logger;
	};
}

#define LOG_DEBUG(fmt, ...)								             \
	        {												             \
				Pionner::LogSystem::log(Pionner::LogLevel::DEBUG, "[%s]:[%s][%s][%d] " fmt,\
							     LIB_TAB, LOCAL_TAG, __FUNCTION__,       \
							     __LINE__, ##__VA_ARGS__);		         \
			}

#define LOG_INFO(fmt, ...)								            \
	        {												            \
				Pionner::LogSystem::log(Pionner::LogLevel::INFO, "[%s]:[%s][%s][%d] " fmt,\
							   LIB_TAB, LOCAL_TAG, __FUNCTION__,        \
							   __LINE__, ##__VA_ARGS__);		        \
			}

#define LOG_WARN(fmt, ...)								            \
	        {												            \
				Pionner::LogSystem::log(Pionner::LogLevel::WARN, "[%s]:[%s][%s][%d] " fmt,\
							    LIB_TAB, LOCAL_TAG, __FUNCTION__,       \
							    __LINE__, ##__VA_ARGS__);		        \
			}

#define LOG_ERR(fmt, ...)								           \
	        {												           \
				Pionner::LogSystem::log(Pionner::LogLevel::ERR, "[%s]:[%s][%s][%d] " fmt,\
							   LIB_TAB, LOCAL_TAG, __FUNCTION__,       \
							   __LINE__, ##__VA_ARGS__);		       \
			}

#define LOG_FATAL(fmt, ...)								            \
	        {												            \
				Pionner::LogSystem::log(Pionner::LogLevel::FATAL, "[%s]:[%s][%s][%d] " fmt,\
							     LIB_TAB, LOCAL_TAG, __FUNCTION__,      \
							     __LINE__, ##__VA_ARGS__);		        \
			}

#endif