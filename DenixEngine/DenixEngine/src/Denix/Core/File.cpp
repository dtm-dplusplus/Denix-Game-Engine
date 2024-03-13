#include "DePch.h"
#include "File.h"

#include "Logger.h"

namespace fs = std::filesystem;

std::string File::Read(const std::string& _relativePath)
{
	std::string fullPath = fs::current_path().parent_path().string() + "\\DenixEngine\\" + _relativePath;

	if (std::ifstream fileStream(fullPath); fileStream.is_open())
	{
		DE_LOG(Log, Info, "Opened file: {}", _relativePath)

		std::stringstream fileString;

		std::string line;
		while (std::getline(fileStream, line))
		{
			fileString << line << "\n";
		}

		return fileString.str();
	}

	DE_LOG(Log, Error, "Failed to open file: {}", fullPath)
	return "";
}

bool File::WriteFile(const std::string& _path, const std::string_view _data)
{

	if (std::ofstream stream(_path); stream.is_open())
	{
		DE_LOG(Log, Trace, "Opened file: {}", _path);

		stream << _data;
		stream.close();
	}

	DE_LOG(Log, Error, "Failed to open file: {}", _path)
		return false;
}

bool File::FileExists(const std::string_view _path)
{

	return std::filesystem::exists(_path.data());
}
