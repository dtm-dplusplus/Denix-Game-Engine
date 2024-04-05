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

		m_Root = fs::current_path().parent_path().string() + R"(\)";
		m_EngineRoot = m_Root + R"(DenixEngine\)";
		m_ProjectRoot = m_Root + m_ProjectName + R"(\)";
		m_ContentRoot = m_ProjectRoot + R"(Content\)";

		DE_LOG(LogFileSubSystem, Trace, "Project Name: {0}", m_ProjectName)
		DE_LOG(LogFileSubSystem, Trace, "Project Root: {0}", m_ProjectRoot)
		DE_LOG(LogFileSubSystem, Trace, "Engine Root: {0}", m_EngineRoot)
		DE_LOG(LogFileSubSystem, Trace, "Root: {0}", m_Root)

		m_Initialized = true;
	}

	void FileSubsystem::Deinitialize()
	{
		DE_LOG(LogFileSubSystem, Trace, "File Subsystem Deinitialized")

			m_Initialized = false;
	}

	std::string FileSubsystem::Read(const std::string& _path, bool _absolute)
	{
		std::string fullPath = s_FileSubsystem->m_EngineRoot + _path;

		if (_absolute)
		{
			fullPath = _path;
		}
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

	std::string FileSubsystem::GetRoot()
	{
		return s_FileSubsystem->m_Root;
	}

	std::string FileSubsystem::GetEngineRoot()
	{
		return s_FileSubsystem->m_EngineRoot;
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