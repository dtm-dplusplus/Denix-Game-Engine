#include "DePch.h"
#include "Engine.h"

#include <ranges>

#include "System/WindowSubSystem.h"
#include "System/ShaderSubSystem.h"
#include "System/SceneSubSystem.h"
#include "System/PhysicsSubSystem.h"
#include "System/UISubSystem.h"

namespace Denix
{
	Engine* Engine::s_Engine{nullptr};

	


	void Engine::Initialize()
	{
		DE_LOG(Log, Trace, "Engine Starting up")

		m_WindowSubSystem = MakeRef<WindowSubSystem>();
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

		DE_LOG(LogEngine, Info, "Engine Initialized")
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void Engine::Deinitialize()
	{
		DE_LOG(LogEngine, Trace, "Engine Shutting Down")

		// Deinitialie SubSystems in the reverse order of initialization
		for (const auto& subSystem : std::views::reverse(m_SubSystems) | std::views::values)
			subSystem->Deinitialize();

		DE_LOG(LogEngine, Trace, "Engine Deinitialized")
	}

	void Engine::Run()
	{
		Initialize();

		while(m_WindowSubSystem->m_Window->IsOpen())
		{
			m_WindowSubSystem->m_Window->PollEvents();

			m_WindowSubSystem->m_Window->ClearBuffer();

			m_SceneSubSystem->Update(0.3f);

			m_WindowSubSystem->m_Window->SwapBuffers();

			m_SceneSubSystem->CleanRubbish();

			SDL_Delay(5); // Delay for physics
		}

		Deinitialize();
	}
}