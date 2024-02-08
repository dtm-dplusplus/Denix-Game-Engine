#include "Log.h"

std::vector<std::shared_ptr<spdlog::logger>> Log::m_Loggers{};
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Log::m_FileSink{};
std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> Log::m_OutputSink{};

