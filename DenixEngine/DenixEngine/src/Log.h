#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <ctime>
#include <sstream>

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
	static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> m_FileSink;

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
		if(const std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>(_name.data(), m_FileSink))
		{
			logger->set_level(spdlog::level::trace);
			logger->set_pattern("%^[%T] [%n]: %v%$");
			logger->flush_on(spdlog::level::trace);
			m_Loggers.push_back(logger);
		}
	}

	static void Start()
	{
		// Get Date time
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);

		std::ostringstream ossDateTime;
		ossDateTime << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");

		std::string path = "Saved\\Logs\\" + ossDateTime.str() + "\\logfile.txt";

		// Create a file sink
		m_FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path, true);


		DX_LOG_CREATE(DX_Log)
		DX_LOG(DX_Log,DX_TRACE, "Logger starting")
	}

	static void Stop()
	{
		DX_LOG(DX_Log,DX_TRACE, "Logger stopping")

		//spdlog::shutdown();
	}
};