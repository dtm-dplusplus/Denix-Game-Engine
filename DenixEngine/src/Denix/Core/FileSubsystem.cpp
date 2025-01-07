#include "FileSubsystem.h"
#include <filesystem>
#include <fstream>
#include <SDL3/SDL_filesystem.h>

namespace fs = std::filesystem;

namespace Denix
{
	void FileSubsystem::Initialize()
	{
		Subsystem::Initialize();
		
		DE_LOG(LogFile, Warn, "Initializing File Subsystem")

		// Get executable path
		const char* p = SDL_GetBasePath();
		if (!p)
		{
			std::string err = SDL_GetError();
			DE_LOG(LogFile, Critical, err)
			throw std::exception(err.c_str());
		}
		
		// Setup Project Paths - We fetch from the binary parent directory
		const fs::path exePath = p;
		SDL_free(const_cast<char*>(p));
		m_BinaryRoot = exePath.parent_path().string() + R"(\)";
		m_ProjectRoot = exePath.parent_path().parent_path().string() + R"(\)";

		// Find Project file
		for (const auto& entry : fs::directory_iterator(m_ProjectRoot))
		{
			if (entry.path().extension() == ".proj")
			{
				m_ProjectFile = entry.path().parent_path().string() + R"(\)";
				break;
			}
		}

		if (m_ProjectFile.empty())
		{
			DE_LOG(LogFile, Critical, "Project file not found")
			throw std::exception("Project file not found");
		}

		// Set Content Paths
		m_ContentRoot = m_ProjectRoot + R"(Content\)";
		m_EngineContentRoot = m_ContentRoot + R"(Engine\)";

		DE_LOG(LogFile, Trace, "Project Root: {0}", m_ProjectRoot)
		DE_LOG(LogFile, Trace, "Binary Root: {0}", m_BinaryRoot)
		DE_LOG(LogFile, Trace, "Content Root: {0}", m_ContentRoot)
		DE_LOG(LogFile, Trace, "Engine Content Root: {0}", m_EngineContentRoot)
		DE_LOG(LogFile, Info, "File Subsystem Initialized")
	}

	void FileSubsystem::Deinitialize()
	{
		DE_LOG(LogFile, Trace, "File Subsystem Deinitializing")
		DE_LOG(LogFile, Trace, "File Subsystem Deinitialized")
	}

	bool FileSubsystem::CopyFileDE(const std::string& _oldPath, const std::string& _newPath)
	{
		try
		{
			fs::copy(_oldPath, _newPath, fs::copy_options::overwrite_existing);
			return true;
		}
		catch (const std::exception& e)
		{
			DE_LOG(LogFile, Error, "Failed to copy file: {0}", e.what())
			return false;
		}
	}

	std::string FileSubsystem::ReadFile(const std::string& _path)
	{
		std::string fullPath =  _path;

		// Skip absolute path check for now
		/*if (_absolute)
		{
			fullPath = _path;
		}*/
		
		if (std::ifstream fileStream(fullPath); fileStream.is_open())
		{
			std::stringstream fileString;

			std::string line;
			while (std::getline(fileStream, line))
			{
				fileString << line << "\n";
			}

			return fileString.str();
		}

		DE_LOG(LogFile, Error, "Failed to open file: {}", fullPath)
			return "";
	}

	bool FileSubsystem::WriteFile(const std::string& _path, const std::string_view _data)
	{
		// Create directory if it doesn't exist
		if(!DirectoryExists(_path))
		{
			DE_LOG(LogFile, Warn, "Directory does not exist: {}", _path)
			CreateDirectoryDE(_path);
		}

		// Open file and write data
		if (std::ofstream stream(_path); stream.is_open())
		{
			stream << _data;
			stream.close();
			return true;
		}

		DE_LOG(LogFile, Error, "Failed to open file: {}", _path)
			return false;
	}

	bool FileSubsystem::FileExists(const std::string& _path)
	{
		return std::filesystem::exists(_path.data());
	}

	bool FileSubsystem::DirectoryExists(const std::string& _path)
	{
		 return  fs::exists(fs::path(_path).parent_path());
	}

	bool FileSubsystem::CreateDirectoryDE(const std::string& _path)
	{
	    try
	    {
	        fs::create_directories(fs::path(_path).parent_path());
    		return true;
	    }
	    catch (const std::exception& e)
	    {
	        DE_LOG(LogFile, Error, "Failed to create directory: {}", e.what());
	        return false;
	    }
	}

	std::string FileSubsystem::FormatPath(const std::string& _path)
	{
		try {
			fs::path inputPath(_path);

			// Check if the provided path is already absolute
			if (IsAbsolute(_path)) return inputPath.make_preferred().string();

			// If the path is relative, combine it with the project root
			fs::path combinedPath = s_Instance->m_ProjectRoot / inputPath;

			// Convert to absolute path and normalize separators
			return fs::absolute(combinedPath).make_preferred().string();
		} catch (const fs::filesystem_error& e) {
			DE_LOG(LogFile, Error, "Failed to format path: {0}", e.what());
			return "";
		}
	}

	std::string FileSubsystem::FormatRelativePath(const std::string& _path)
	{
		// Check if the input path is already relative
		if (!IsAbsolute(_path)) return _path;

		// Calculate the relative path
		std::filesystem::path relativePath = fs::relative(_path, s_Instance->m_ProjectRoot);

		return relativePath.make_preferred().string();
	}

	bool FileSubsystem::IsAbsolute(const std::string& _path)
	{
		// Check if the input path starts with the project root path
		return _path.find(s_Instance->m_ProjectRoot) == 0;
	}
}
