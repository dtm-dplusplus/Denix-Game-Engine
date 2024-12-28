#pragma once
#include "Denix/System/SubSystem.h"
#include "Denix/Core.h"

namespace Denix
{ 
    class FileSubsystem: public Subsystem<FileSubsystem>
    {
	public:
		FileSubsystem(std::string _projectName) : m_ProjectName(std::move(_projectName))
		{
			DE_LOG_CREATE(LogFile)
		}

		~FileSubsystem() override = default;

		FileSubsystem(const FileSubsystem& _other) = delete;
		FileSubsystem(FileSubsystem&& _other) noexcept = delete;
		FileSubsystem& operator=(const FileSubsystem& _other) = delete;
		FileSubsystem& operator=(FileSubsystem&& _other) noexcept = delete;
    	
		static std::string ReadFile(const std::string& _path);

		static bool WriteFile(const std::string& _path, const std::string_view _data);

		static bool FileExists(const std::string& _path);

    	static std::string GetProjectRoot() { return s_Instance->m_ProjectRoot; }

    	static std::string GetContentRoot() { return s_Instance->m_ContentRoot; }

		static std::string GetEngineContentRoot() { return s_Instance->m_EngineContentRoot; }

		static std::string GetProjectName()	{ return s_Instance->m_ProjectName; }

		 static bool DirectoryExists(const std::string& _path);

    	static bool CreateDirectory(const std::string& _path);

	private:
    	void Initialize() override;

    	void Deinitialize() override;
    	
		std::string m_ProjectName;

    	// Root of the project
		std::string m_ProjectRoot;

    	// Root of the content
		std::string m_ContentRoot;

    	// Root of the engine content 
    	std::string m_EngineContentRoot;

		friend class Engine;

	};
}
