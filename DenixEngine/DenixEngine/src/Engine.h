#pragma once

#include "Core.h"



namespace Denix
{
	class Engine
	{
	public:

		Engine();
		~Engine();

		void Initialize();
		void Deinitialize();

		void Run();

		static Engine& Get() { return *s_Engine; }

	private:
		static Engine* s_Engine;

		//std::vector<Ref<SubSystem>>  m_SubSystems;

		Ref<class WindowSubSystem> m_WindowSubSystem;

		Ref<class SceneSubSystem> m_SceneSubSystem;

		Ref<class ShaderSubSystem> m_ShaderSubSystem;
		Ref<class UISubSystem> m_UISubSytem;
	};
}
