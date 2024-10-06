#include "FileSubsystem.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace Denix
{
	FileSubsystem* FileSubsystem::s_FileSubsystem = nullptr;


	void FileSubsystem::Initialize()
	{
		DE_LOG(LogFileSubSystem, Trace, "File Subsystem Initialized")

		// search the directory for the project file
		// current directory
		std::string currentPath = fs::current_path().string();
		for (const auto& entry : std::filesystem::recursive_directory_iterator(fs::current_path()))
		{
			if(entry.is_regular_file())
			{
				if(entry.path().filename().string().find("deproj") != std::string::npos)
				{
					m_ProjectRoot = entry.path().parent_path().string() + R"(\)";
					break;
				}
			}
		}
		m_ContentRoot = m_ProjectRoot + R"(Content\)";
		m_EngineContentRoot = m_ContentRoot + R"(Engine\)";

		DE_LOG(LogFileSubSystem, Trace, "Project Name: {0}", m_ProjectName)
		DE_LOG(LogFileSubSystem, Trace, "Project Root: {0}", m_ProjectRoot)

		m_Initialized = true;
	}

	void FileSubsystem::Deinitialize()
	{
		DE_LOG(LogFileSubSystem, Trace, "File Subsystem Deinitialized")

			m_Initialized = false;
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
			DE_LOG(LogFileSubSystem, Info, "Opened file: {}", _path)

				std::stringstream fileString;

			std::string line;
			while (std::getline(fileStream, line))
			{
				fileString << line << "\n";
			}

			return fileString.str();
		}

		DE_LOG(LogFileSubSystem, Error, "Failed to open file: {}", fullPath)
			return "";
	}

	bool FileSubsystem::WriteFile(const std::string& _path, const std::string_view _data)
	{
		// Create directory if it doesn't exist
		if(!DirectoryExists(_path))
		{
			DE_LOG(LogFileSubSystem, Error, "Directory does not exist: {}", _path)
			CreateDirectoryA(_path);
		}

		// Open file and write data
		if (std::ofstream stream(_path); stream.is_open())
		{
			stream << _data;
			stream.close();
			return true;
		}

		DE_LOG(LogFileSubSystem, Error, "Failed to open file: {}", _path)
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

	bool FileSubsystem::CreateDirectory(const std::string& _path)
	{
	    try
	    {
	        fs::create_directories(fs::path(_path).parent_path());
    		return true;
	    }
	    catch (const std::exception& e)
	    {
	        DE_LOG(LogFileSubSystem, Error, "Failed to create directory: {}", e.what());
	        return false;
	    }
	}
}