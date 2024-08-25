#ifndef __PIONNER_BASE_UTILS_LOG_SYSTEM_H__
#define __PIONNER_BASE_UTILS_LOG_SYSTEM_H__

#include <base/defs/FunDef.h>

#ifdef LIB_TAB
#undef LIB_TAB
#endif
#define LIB_TAB "Pionner"

namespace spdlog
{
	class logger;
}

namespace pio
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
		static void Initialize();
		static void Shutdown();

		static void Log(LogLevel level, const char *fmt, ...);

	private:
		LogSystem() {};
		~LogSystem() = default;

		static std::shared_ptr<spdlog::logger> g_logger;
	};
}

//Log level debug
#define LOGD(fmt, ...)	\
	        {\
				pio::LogSystem::Log(pio::LogLevel::DEBUG, "[%s]:[%s][%s][%d] " fmt,\
							     LIB_TAB, LOCAL_TAG, __FUNCTION__,\
							     __LINE__, ##__VA_ARGS__);\
			}

#define LOGD_FN_ENTER	\
	        {\
				pio::LogSystem::Log(pio::LogLevel::DEBUG, "[%s]:[%s][%s][%d] enter",\
							    LIB_TAB, LOCAL_TAG, __FUNCTION__, __LINE__);\
			}

#define LOGD_FN_EXIT	\
	        {\
				pio::LogSystem::Log(pio::LogLevel::DEBUG, "[%s]:[%s][%s][%d] exit",\
							    LIB_TAB, LOCAL_TAG, __FUNCTION__, __LINE__);\
			}

//Log level info
#define LOGI(fmt, ...)	\
	        {\
				pio::LogSystem::Log(pio::LogLevel::INFO, "[%s]:[%s][%s][%d] " fmt,\
							   LIB_TAB, LOCAL_TAG, __FUNCTION__,\
							   __LINE__, ##__VA_ARGS__);\
			}

#define LOGI_FN_ENTER	\
	        {\
				pio::LogSystem::Log(pio::LogLevel::INFO, "[%s]:[%s][%s][%d] enter",\
							    LIB_TAB, LOCAL_TAG, __FUNCTION__, __LINE__)\
			}

#define LOGI_FN_EXIT	\
	        {\
				pio::LogSystem::Log(pio::LogLevel::INFO, "[%s]:[%s][%s][%d] exit",\
							    LIB_TAB, LOCAL_TAG, __FUNCTION__, __LINE__);\
			}

//Log level warn
#define LOGW(fmt, ...)	\
	        {\
				pio::LogSystem::Log(pio::LogLevel::WARN, "[%s]:[%s][%s][%d] " fmt,\
							    LIB_TAB, LOCAL_TAG, __FUNCTION__,\
							    __LINE__, ##__VA_ARGS__);\
			}

//Log level error
#define LOGE(fmt, ...)	\
	        {\
				pio::LogSystem::Log(pio::LogLevel::ERR, "[%s]:[%s][%s][%d] " fmt,\
							   LIB_TAB, LOCAL_TAG, __FUNCTION__,\
							   __LINE__, ##__VA_ARGS__);\
			}

//Log level fatal
#define LOGF(fmt, ...)	\
	        {\
				pio::LogSystem::Log(pio::LogLevel::FATAL, "[%s]:[%s][%s][%d] " fmt,\
							     LIB_TAB, LOCAL_TAG, __FUNCTION__,\
							     __LINE__, ##__VA_ARGS__);\
			}

#endif