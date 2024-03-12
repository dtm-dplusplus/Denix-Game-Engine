#pragma once

#include <map>

#include "Core.h"



namespace Denix
{
	class SubSystem;

	class Engine
	{
	public:

		Engine()
		{
			s_Engine = this;
		}

		~Engine()
		{
			s_Engine = nullptr;
		}

		void Initialize();
		void Deinitialize();

		void Run();

		static Engine& Get() { return *s_Engine; }

	private:
		static Engine* s_Engine;

		std::unordered_map<std::string, Ref<SubSystem>>  m_SubSystems;

		Ref<class WindowSubSystem> m_WindowSubSystem;

		Ref<class SceneSubSystem> m_SceneSubSystem;

		Ref<class PhysicsSubSystem> m_PhysicsSubSystem;

		Ref<class ShaderSubSystem> m_ShaderSubSystem;

		Ref<class RendererSubSystem> m_RendererSubSystem;

		Ref<class UISubSystem> m_UISubSytem;
	};
}