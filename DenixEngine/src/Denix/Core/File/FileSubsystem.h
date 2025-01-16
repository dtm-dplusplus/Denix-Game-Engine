#pragma once
#include "Denix/Core/Subsystem.h"
#include "Denix/Core.h"

namespace Denix
{ 
    class FileSubsystem: public Subsystem<FileSubsystem>
    {
	public:
		FileSubsystem() = default;

		~FileSubsystem() override = default;

		FileSubsystem(const FileSubsystem& _other) = delete;
		FileSubsystem(FileSubsystem&& _other) noexcept = delete;
		FileSubsystem& operator=(const FileSubsystem& _other) = delete;
		FileSubsystem& operator=(FileSubsystem&& _other) noexcept = delete;

    	
    	static std::string GetProjectRoot() { return s_Instance->m_ProjectRoot; }

    	static std::string GetContentRoot() { return s_Instance->m_ContentRoot; }

    	static std::string GetEngineContentRoot() { return s_Instance->m_EngineContentRoot; }

    	
    	static bool CopyFileDE(const std::string& _oldPath, const std::string& _newPath);
    	
		static std::string ReadFile(const std::string& _path);

		static bool WriteFile(const std::string& _path, std::string_view _data);

		static bool FileExists(const std::string& _path);

		 static bool DirectoryExists(const std::string& _path);

    	static bool CreateDirectoryDE(const std::string& _path);

    	
    	static std::string FormatPath(const std::string& _path);
    	
		static std::string FormatRelativePath(const std::string& _path);

    	static bool IsAbsolute(const std::string& _path);
		
	private:

    	
		void Initialize() override;

    	void Deinitialize() override;
    	
    	// Root of the project
		std::string m_ProjectRoot;

    	std::string m_ProjectFile;

    	std::string m_BinaryRoot;
    	
    	// Root of the content
		std::string m_ContentRoot;

    	// Root of the engine content 
    	std::string m_EngineContentRoot;

		friend class Engine;

	};
}
