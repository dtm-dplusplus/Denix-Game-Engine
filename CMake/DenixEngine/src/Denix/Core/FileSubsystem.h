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

		static std::string Read(const std::string& _path, bool _absolute = false);

		static bool WriteFile(const std::string& _path, const std::string_view _data);

		static bool FileExists(const std::string& _path);

		static std::string GetRoot();

		static std::string GetEngineRoot();
		
		static std::string GetEngineContentRoot();

		static std::string GetProjectRoot();

		static std::string GetProjectName();

		static std::string GetUserContentRoot();

	public:
		void Initialize() override;

		void Deinitialize() override;

		static FileSubsystem* Get() { return s_FileSubsystem; }


	private:
		static FileSubsystem* s_FileSubsystem;
		
		std::string m_Root;

		std::string m_EngineRoot;

		std::string m_EngineContentRoot;

		std::string m_ProjectName;

		std::string m_ProjectRoot;

		std::string m_UserContentRoot;

		friend class Engine;

	};
}
