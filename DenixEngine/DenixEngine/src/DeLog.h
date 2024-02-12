#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <ctime>
#include <sstream>

#define Trace spdlog::level::trace
#define Debug spdlog::level::debug
#define Info spdlog::level::info
#define Warn spdlog::level::warn
#define Error spdlog::level::err
#define Critical spdlog::level::critical

#define DE_LOG_CREATE(name) DeLog::CreateLogger(#name);
#define DE_LOG(logger, level, message, ...) DeLog::GetLogger(#logger)->log(level, message, ##__VA_ARGS__);

class DeLog final
{
public:
	static std::vector<std::shared_ptr<spdlog::logger>> m_Loggers;
	static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> m_FileSink;
	static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> m_OutputSink;


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
		std::vector<spdlog::sink_ptr> sinks = { m_FileSink, m_OutputSink };
		if (const std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>(_name.data(), sinks.begin(), sinks.end()))
		{
			logger->set_level(spdlog::level::trace);
			logger->set_pattern("%^[%T] [%n]: %v%$");
			logger->flush_on(spdlog::level::trace);
			m_Loggers.push_back(logger);

			// Atomic error
			// DE_LOG(_name.data(), Trace, "Created logger: %s", _name.data())
		}
		else
		{
			DE_LOG(log, Error, "Failed to create logger: %s", _name.data())
		}
	}

	static void Start()
	{
		// Get Date time - Need to abstract this in the future
		const auto t = std::time(nullptr);
		const auto tm = *std::localtime(&t);

		std::ostringstream ossDateTime;
		ossDateTime << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");

		// Should be stored in a config file/ class variable
		std::string path = "Saved\\Logs\\" + ossDateTime.str() + "\\logfile.txt";

		// Create file sink & output sink
		m_FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path, true);
		m_OutputSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

		// Default log catergories Should be stored in a config file
		DE_LOG_CREATE(Log)
		DE_LOG_CREATE(LogGL)
		DE_LOG_CREATE(LogShader)
		DE_LOG(Log, Trace, " Denix Engine Logger starting")
	}

	static void Stop()
	{
		// DE_LOG(DX_Log,Trace, "Logger stopping")

		//spdlog::shutdown();
	}
};