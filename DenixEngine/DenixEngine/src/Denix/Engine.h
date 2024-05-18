#pragma once

#include "Core.h"

int main(int argc, char** argv);

namespace Denix
{
	class Subsystem;

	class Engine
	{
	public:

		Engine();
		virtual  ~Engine();

		void Initialize();
		void Deinitialize();

		void Run();

		static Engine& Get() { return *s_Engine; }

		std::string GetProjectName() const { return m_ProjectName; }

	public:
		virtual void PreInitialize();
		virtual void PostInitialize();

		// TEMP
		virtual void RestartScene() {}

	protected:
		std::string m_ProjectName;

		
	private:
		static Engine* s_Engine;

		std::vector< Ref<Subsystem>> m_SubsystemOrder;
		std::unordered_map<std::string, Ref<Subsystem>>  m_Subsystems;

		Ref<class TimerSubsystem> m_TimerSubSystem;

		Ref<class WindowSubsystem> m_WindowSubsystem;

		Ref<class FileSubsystem> m_FileSubSystem;

		Ref<class ResourceSubsystem> m_ResourceSubSystem;

		Ref<class SceneSubsystem> m_SceneSubSystem;

		Ref<class PhysicsSubsystem> m_PhysicsSubSystem;

		Ref<class UISubsystem> m_UISubsystem;

		Ref<class RendererSubsystem> m_RendererSubSystem;

		Ref<class EditorSubsystem> m_EditorSubSystem;

		Ref<class InputSubsystem> m_InputSubsystem;

		friend int ::main(int argc, char** argv);
	};

	// Defined in client
	Engine* CreateEngine();
}
