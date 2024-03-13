#include "DePch.h"
#include "Engine.h"
#include <iostream>

#include "Core/DeLog.h"
#include "System/UISubSystem.h"

#include <ranges>

#include "System/WindowSubSystem.h"
<<<<<<< HEAD:DenixEngine/DenixEngine/src/Denix/Engine.cpp
//#include "System/ShaderSubSystem.h"
//#include "System/SceneSubSystem.h"
//#include "System/UISubSystem.h"
=======
#include "System/ShaderSubSystem.h"
#include "System/SceneSubSystem.h"
#include "System/PhysicsSubSystem.h"
#include "System/UISubSystem.h"
>>>>>>> main:DenixEngine/DenixEngine/src/Engine.cpp

namespace Denix
{
	Engine* Engine::s_Engine{nullptr};

<<<<<<< HEAD:DenixEngine/DenixEngine/src/Denix/Engine.cpp
	Engine::Engine()
	{
		s_Engine = this;
		DeLog::Start();
		
	}

	Engine::~Engine()
	{
		s_Engine = nullptr;
	}
=======
	
>>>>>>> main:DenixEngine/DenixEngine/src/Engine.cpp

	void Engine::Print()
	{
		while (true) {}
			
			//std::cout << "Post build" << std::endl;
	}

	void Engine::Initialize()
	{
		DE_LOG(Log, Trace, "Engine Starting up")

		m_WindowSubSystem = MakeRef<WindowSubSystem>();
<<<<<<< HEAD:DenixEngine/DenixEngine/src/Denix/Engine.cpp
		m_WindowSubSystem->Initialize();

	//	m_ShaderSubSystem = MakeRef<ShaderSubSystem>();
	//	m_ShaderSubSystem->Initialize();

		m_UISubSytem = MakeRef<UISubSystem>();
		m_UISubSytem->Initialize();

	//	m_SceneSubSystem = MakeRef<SceneSubSystem>();
	//	m_SceneSubSystem->Initialize();*/
=======
		m_SubSystems["Window"] = m_WindowSubSystem;

		m_ShaderSubSystem = MakeRef<ShaderSubSystem>();
		m_SubSystems["Shader"] = m_ShaderSubSystem;

		m_UISubSytem = MakeRef<UISubSystem>();
		m_SubSystems["UI"] = m_UISubSytem;

		m_PhysicsSubSystem = MakeRef<PhysicsSubSystem>();
		m_SubSystems["Physics"] = m_PhysicsSubSystem;

		m_SceneSubSystem = MakeRef<SceneSubSystem>();
		m_SubSystems["Scene"] = m_SceneSubSystem;

		// Order of initialization is defined above
		for(const auto& subSystem : m_SubSystems | std::views::values)
			subSystem->Initialize();
>>>>>>> main:DenixEngine/DenixEngine/src/Engine.cpp

		DE_LOG(LogEngine, Info, "Engine Initialized")
	}

	//// ReSharper disable once CppMemberFunctionMayBeConst
	void Engine::Deinitialize()
	{
		DE_LOG(LogEngine, Trace, "Engine Shutting Down")

<<<<<<< HEAD:DenixEngine/DenixEngine/src/Denix/Engine.cpp
	//	/*m_SceneSubSystem->Deinitialize();
		m_UISubSytem->Deinitialize();
	//	m_ShaderSubSystem->Deinitialize();
		m_WindowSubSystem->Deinitialize();
=======
		// Deinitialie SubSystems in the reverse order of initialization
		for (const auto& subSystem : std::views::reverse(m_SubSystems) | std::views::values)
			subSystem->Deinitialize();
>>>>>>> main:DenixEngine/DenixEngine/src/Engine.cpp

		DE_LOG(LogEngine, Trace, "Engine Deinitialized")
	}

	void Engine::Run()
	{
		Initialize();

		while(m_WindowSubSystem->m_Window->IsOpen())
		{
			m_WindowSubSystem->m_Window->PollEvents();

			m_WindowSubSystem->m_Window->ClearBuffer();

		//	m_SceneSubSystem->Update(0.3f);

			m_WindowSubSystem->m_Window->SwapBuffers();

<<<<<<< HEAD:DenixEngine/DenixEngine/src/Denix/Engine.cpp
		//	SDL_Delay(5); // Delay for physics
=======
			m_SceneSubSystem->CleanRubbish();

			SDL_Delay(5); // Delay for physics
>>>>>>> main:DenixEngine/DenixEngine/src/Engine.cpp
		}

		//while (true)
		//{
		//	/*std::cout << "Engine Running" << std::endl;*/
		//	DE_LOG(Log, Info, "Lol")
		//}

		Deinitialize();
	}
}