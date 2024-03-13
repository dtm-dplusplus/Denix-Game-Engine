#pragma once

#include <map>

#include "Core.h"

int main(int argc, char** argv);

namespace Denix
{
	class SubSystem;

	class Engine
	{
	public:

<<<<<<< HEAD:DenixEngine/DenixEngine/src/Denix/Engine.h
		Engine();
		virtual  ~Engine();
=======
		Engine()
		{
			s_Engine = this;
		}

		~Engine()
		{
			s_Engine = nullptr;
		}
>>>>>>> main:DenixEngine/DenixEngine/src/Engine.h

		void Initialize();
		void Deinitialize();

		void Run();

		static Engine& Get() { return *s_Engine; }

		virtual void Test() {}
		static void Print();
	private:
		static Engine* s_Engine;

		std::unordered_map<std::string, Ref<SubSystem>>  m_SubSystems;

		Ref<class WindowSubSystem> m_WindowSubSystem;

		/*Ref<class SceneSubSystem> m_SceneSubSystem;

		Ref<class ShaderSubSystem> m_ShaderSubSystem;*/

<<<<<<< HEAD:DenixEngine/DenixEngine/src/Denix/Engine.h
=======
		Ref<class PhysicsSubSystem> m_PhysicsSubSystem;

		Ref<class ShaderSubSystem> m_ShaderSubSystem;

>>>>>>> main:DenixEngine/DenixEngine/src/Engine.h
		Ref<class UISubSystem> m_UISubSytem;

		friend int ::main(int argc, char** argv);
	};
<<<<<<< HEAD:DenixEngine/DenixEngine/src/Denix/Engine.h

	// Defined in client
	Engine* CreateEngine();
}
=======
}
>>>>>>> main:DenixEngine/DenixEngine/src/Engine.h
