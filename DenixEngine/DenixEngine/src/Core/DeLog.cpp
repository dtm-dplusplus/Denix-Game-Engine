#include "DeLog.h"

std::vector<std::shared_ptr<spdlog::logger>> DeLog::m_Loggers{};
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> DeLog::m_FileSink{};
std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> DeLog::m_OutputSink{};

