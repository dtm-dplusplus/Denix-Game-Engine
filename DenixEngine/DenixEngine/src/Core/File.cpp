#include "DePch.h"
#include "File.h"

#include <filesystem>
#include <fstream>
#include <sstream>

// std::string File::m_ResDir{};

std::string File::Read(const std::string_view _path)
{
	if (std::ifstream fileStream(_path.data()); fileStream.is_open())
	{
		DE_LOG(Log, Trace, "Opened file: {}", _path)

		std::stringstream fileString;

		std::string line;
		while (std::getline(fileStream, line))
			fileString << line << "\n";

		return fileString.str();
	}

	DE_LOG(Log, Error, "Failed to open file: {}", _path)
	return "";
}

 bool File::WriteFile(const std::string_view _path, const std::string_view _data)
 {
	 
	 if (std::ofstream stream(_path.data()); stream.is_open())
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
