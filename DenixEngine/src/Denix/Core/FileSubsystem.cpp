#include "FileSubsystem.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace Denix
{
	void FileSubsystem::Initialize()
	{
		Subsystem::Initialize();
		
		DE_LOG(LogFile, Warn, "Initializing File Subsystem")
		// Find the project file and set the project root
		// We need the project name to find the project file
		if(m_ProjectName.empty()) 
		{
			const std::string errorMessage = "Project name is empty";
			DE_LOG(LogFile, Error, errorMessage)
			throw std::runtime_error(errorMessage.c_str());
		}

		// Set name of the project executable
		const std::string projectExecutable = m_ProjectName + ".exe";
		const std::string projectFile = m_ProjectName + ".proj";
		
		// Get Current path
		// If the project is launched from the IDE, the current path is the solution root
		// If the project is launched from the executable, the current path is the executable root
		const fs::path currentPath = fs::current_path();
		DE_LOG(LogFile, Trace, "Starting search for project in: {}", currentPath.string())
		
		// Check if the project is launched from the executable
		if(FileExists(currentPath.string() + R"(\)" + projectExecutable))
		{
			// We are in the executable root, use the parent path to get the project root
			m_ProjectRoot = currentPath.parent_path().parent_path().string() + R"(\)";
			DE_LOG(LogFile, Trace, "Found Project root via executable")
		}
		else
		{
			// We are in the solution root, search for the project root
			const fs::path projectFilePath = currentPath.string()  + R"(\)" + m_ProjectName + R"(\)"+ projectFile;
			if(FileExists(projectFilePath.string()))
			{
				m_ProjectRoot = projectFilePath.parent_path().string() + R"(\)";
				DE_LOG(LogFile, Trace, "Found Project root via solution")
			}
		}

		// Check if project root was found
		if (m_ProjectRoot.empty())
		{
			const char* errorMessage = "Project file not found via executable or solution";
			DE_LOG(LogFile, Critical, errorMessage)
			throw std::exception(errorMessage);
		}

		// Set content roots
		m_ContentRoot = m_ProjectRoot + R"(Content\)";
		m_EngineContentRoot = m_ContentRoot + R"(Engine\)";

		DE_LOG(LogFile, Trace, "Project Root: {0}", m_ProjectRoot)
		DE_LOG(LogFile, Trace, "Content Root: {0}", m_ContentRoot)
		DE_LOG(LogFile, Trace, "Engine Content Root: {0}", m_EngineContentRoot)
		DE_LOG(LogFile, Info, "File Subsystem Initialized")
	}

	void FileSubsystem::Deinitialize()
	{
		DE_LOG(LogFile, Trace, "File Subsystem Deinitialized")
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
			CreateDirectoryA(_path);
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

	bool FileSubsystem::CreateDirectory(const std::string& _path)
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
}