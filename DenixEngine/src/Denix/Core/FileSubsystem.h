#pragma once
#include "Denix/System/SubSystem.h"
#include "Denix/Core.h"

namespace Denix
{ 
    class FileSubsystem: public Subsystem
    {
	public:
		FileSubsystem(const std::string& _projectName) : m_ProjectName(_projectName)
		{
			DE_LOG_CREATE(LogFileSubsystem)
			s_FileSubsystem = this;
		}

		~FileSubsystem() override
		{
			s_FileSubsystem = nullptr;
		}

		static std::string ReadFile(const std::string& _path);

		static bool WriteFile(const std::string& _path, const std::string_view _data);

		static bool FileExists(const std::string& _path);

    	static std::string GetProjectRoot() { return s_FileSubsystem->m_ProjectRoot; }

    	static std::string GetContentRoot() { return s_FileSubsystem->m_ContentRoot; }

		static std::string GetEngineContentRoot() { return s_FileSubsystem->m_EngineContentRoot; }

		static std::string GetProjectName()	{ return s_FileSubsystem->m_ProjectName; }

		 static bool DirectoryExists(const std::string& _path);

    	static bool CreateDirectory(const std::string& _path);
	public:
    	
		void Initialize() override;

		void Deinitialize() override;

		static FileSubsystem* Get() { return s_FileSubsystem; }


	private:
		static FileSubsystem* s_FileSubsystem;
		
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
