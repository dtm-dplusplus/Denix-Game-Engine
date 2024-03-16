#pragma once

#include "Core.h"

int main(int argc, char** argv);

namespace Denix
{
	class SubSystem;

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

		std::unordered_map<std::string, Ref<SubSystem>>  m_SubSystems;

		Ref<class WindowSubSystem> m_WindowSubSystem;

		Ref<class SceneSubSystem> m_SceneSubSystem;

		Ref<class ShaderSubSystem> m_ShaderSubSystem;

		Ref<class PhysicsSubSystem> m_PhysicsSubSystem;

		Ref<class UISubSystem> m_UISubSytem;

		Ref<class RendererSubSystem> m_RendererSubSystem;

		Ref<class EditorSubsystem> m_EditorSubSystem;

		friend int ::main(int argc, char** argv);
	};

	// Defined in client
	Engine* CreateEngine();
}
