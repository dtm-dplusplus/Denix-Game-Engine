#include "Logger.h"

std::vector<std::shared_ptr<spdlog::logger>> Logger::m_Loggers{};
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Logger::m_FileSink{};
std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> Logger::m_OutputSink{};

