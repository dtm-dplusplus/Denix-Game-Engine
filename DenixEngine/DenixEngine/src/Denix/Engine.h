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

	public:
		virtual void PreInitialize();
		virtual void PostInitialize();

		// TEMP
		virtual void RestartScene() {}
	private:
		static Engine* s_Engine;

		std::unordered_map<std::string, Ref<Subsystem>>  m_Subsystems;

		Ref<class WindowSubsystem> m_WindowSubSystem;

		Ref<class SceneSubsystem> m_SceneSubSystem;

		Ref<class ShaderSubsystem> m_ShaderSubSystem;

		Ref<class PhysicsSubsystem> m_PhysicsSubSystem;

		Ref<class UISubsystem> m_UISubSytem;

		Ref<class RendererSubsystem> m_RendererSubSystem;

		Ref<class EditorSubsystem> m_EditorSubSystem;

		Ref<class InputSubsystem> m_InputSubsystem;

		friend int ::main(int argc, char** argv);
	};

	// Defined in client
	Engine* CreateEngine();
}
