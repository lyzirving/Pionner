#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <cassert>
#include <cstdarg>

#include "LogSystem.h"

namespace pio
{
	std::shared_ptr<spdlog::logger> LogSystem::g_logger{ nullptr };

	void LogSystem::Initialize()
	{
		if (g_logger)
			return;

		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		consoleSink->set_level(spdlog::level::debug);

		const spdlog::sinks_init_list sinkList = { consoleSink };

		spdlog::init_thread_pool(8192, 1);

		g_logger = std::make_shared<spdlog::async_logger>("logger",
			sinkList.begin(), sinkList.end(),
			spdlog::thread_pool(),
			spdlog::async_overflow_policy::block);

		g_logger->set_level(spdlog::level::debug);
		spdlog::register_logger(g_logger);
	}

	void LogSystem::Shutdown()
	{
		if (g_logger)
		{
			g_logger->flush();
			spdlog::drop_all();
			g_logger.reset();
		}
	}

	void LogSystem::Log(LogLevel level, const char* fmt, ...)
	{
		if (!g_logger)
			assert(0);

		char buf[512] = { 0 };
		va_list ap;
		va_start(ap, fmt);
		vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
		va_end(ap);

		switch (level)
		{
		case pio::DEBUG:
			g_logger->debug(buf);
			break;
		case pio::INFO:
			g_logger->info(buf);
			break;
		case pio::WARN:
			g_logger->warn(buf);
			break;
		case pio::ERR:
			g_logger->error(buf);
			break;
		case pio::FATAL:
			g_logger->critical(buf);
			break;
		default:
			break;
		}
	}
}