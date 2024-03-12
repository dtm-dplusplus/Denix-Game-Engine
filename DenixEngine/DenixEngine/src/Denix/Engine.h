#pragma once

#include "Core.h"

int main(int argc, char** argv);

namespace Denix
{
	class Engine
	{
	public:

		Engine();
		virtual  ~Engine();

		void Initialize();
		void Deinitialize();

		void Run();

		static Engine& Get() { return *s_Engine; }

		virtual void Test() {}
		static void Print();
	private:
		static Engine* s_Engine;

		//std::vector<Ref<SubSystem>>  m_SubSystems;

		Ref<class WindowSubSystem> m_WindowSubSystem;

		/*Ref<class SceneSubSystem> m_SceneSubSystem;

		Ref<class ShaderSubSystem> m_ShaderSubSystem;*/

		Ref<class UISubSystem> m_UISubSytem;

		friend int ::main(int argc, char** argv);
	};

	// Defined in client
	Engine* CreateEngine();
}
