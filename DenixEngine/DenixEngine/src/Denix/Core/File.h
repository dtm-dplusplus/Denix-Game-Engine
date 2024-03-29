#pragma once

#include <string>

// Header Contents
class File
{

public:
	static std::string Read(const std::string& _path, bool _absolute = false);

	static bool WriteFile(const std::string& _path, const std::string_view _data);

	static bool FileExists(std::string_view _path);

};