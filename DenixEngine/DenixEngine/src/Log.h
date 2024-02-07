#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#define DX_TRACE spdlog::level::trace
#define DX_DEBUG spdlog::level::debug
#define DX_INFO spdlog::level::info
#define DX_WARN spdlog::level::warn
#define DX_ERROR spdlog::level::err
#define DX_CRITICAL spdlog::level::critical

#define DX_LOG_CREATE(name) Log::CreateLogger(#name);
#define DX_LOG(logger, level, message, ...) Log::GetLogger(#logger)->log(level, message, ##__VA_ARGS__);


class Log
{
public:
	static std::vector<std::shared_ptr<spdlog::logger>> m_Loggers;

	static std::shared_ptr<spdlog::logger> GetLogger(const std::string_view _name)
	{
		for(auto& logger : m_Loggers)
		{
			if (logger->name() == _name)
				return logger;
		}
		return nullptr;
	}

	static void CreateLogger(const std::string_view _name)
	{
		if(const std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt(_name.data()))
		{
			logger->set_level(spdlog::level::trace);
			logger->set_pattern("%^[%T] [%n]: %v%$");
			logger->flush_on(spdlog::level::trace);
			m_Loggers.push_back(logger);
		}
	}

	static void Start()
	{
		DX_LOG_CREATE(DX_Log)
		DX_LOG(DX_Log,DX_TRACE, "Logger starting")
	}

	static void Stop()
	{
		DX_LOG(DX_Log,DX_TRACE, "Logger stopping")

		//spdlog::shutdown();
	}
};


