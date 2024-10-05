#pragma once
#include "Denix/System/SubSystem.h"
#include "Denix/Core.h"

namespace Denix
{ 
    class FileSubsystem: public Subsystem
    {
	public:
		FileSubsystem()
		{
			DE_LOG_CREATE(LogFileSubSystem)
			s_FileSubsystem = this;
		}

		~FileSubsystem() override
		{
			s_FileSubsystem = nullptr;
		}

		static std::string ReadFile(const std::string& _path);

		static bool WriteFile(const std::string& _path, const std::string_view _data);

		static bool FileExists(const std::string& _path);

    	static std::string GetProjectRoot();

    	static std::string GetContentRoot();

		static std::string GetEngineContentRoot();

		static std::string GetProjectName();


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
