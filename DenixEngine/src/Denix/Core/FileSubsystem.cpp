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

		m_ProjectRoot = fs::current_path().string() + R"(\)";
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
		if (std::ofstream stream(_path); stream.is_open())
		{
			DE_LOG(LogFileSubSystem, Trace, "Opened file: {}", _path);

			stream << _data;
			stream.close();
		}

		DE_LOG(Log, Error, "Failed to open file: {}", _path)
			return false;
	}

	bool FileSubsystem::FileExists(const std::string& _path)
	{

		return std::filesystem::exists(_path.data());
	}

	std::string FileSubsystem::GetEngineContentRoot()
	{
		return s_FileSubsystem->m_EngineContentRoot;
	}

	std::string FileSubsystem::GetProjectName()
	{
		return s_FileSubsystem->m_ProjectName;
	}

	std::string FileSubsystem::GetContentRoot()
	{
		return s_FileSubsystem->m_ContentRoot;
	}

	std::string FileSubsystem::GetProjectRoot()
	{
		return s_FileSubsystem->m_ProjectRoot;
	}

}