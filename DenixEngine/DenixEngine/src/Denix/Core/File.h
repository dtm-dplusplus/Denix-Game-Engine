#pragma once

#include <string>

// Header Contents
class File
{

public:
	static std::string Read(std::string_view _path);

	static bool WriteFile(std::string_view _path, std::string_view _data);

	static bool FileExists(std::string_view _path);

};