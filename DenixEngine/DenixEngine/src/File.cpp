#include "DePch.h"
#include "File.h"

#include <filesystem>
#include <fstream>
#include <sstream>

// std::string File::m_ResDir{};

std::string File::Read(const std::string_view _path)
{
	std::ifstream fileStream(_path.data());
	std::stringstream fileString;

	std::string line;
	while (std::getline(fileStream, line))
		fileString << line << "\n";

	return fileString.str();
}

 bool File::WriteFile(const std::string_view _path, const std::string_view _data)
 {
	 std::ofstream stream(_path.data());
	 if (!stream.is_open())
	 {
		 DE_LOG(Log, Error, "Failed to open file: {0}", _path);
		 return false;
	 }

	 stream << _data;
	 stream.close();

	 return true;
 }

bool File::FileExists(const std::string_view _path)
{
	
	return std::filesystem::exists(_path.data());
}
