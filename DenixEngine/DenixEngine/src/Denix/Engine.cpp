#include "depch.h"
#include "Engine.h"


#include "System/WindowSubsystem.h"
#include "System/ShaderSubsystem.h"
#include "System/UISubsystem.h"
#include "System/PhysicsSubsystem.h"
#include "System/SceneSubsystem.h"
#include "System/RendererSubsystem.h"
#include "Input/InputSubsystem.h"
#include "Editor/EditorSubsystem.h"

namespace Denix
{
	Engine* Engine::s_Engine{nullptr};

	Engine::Engine()
	{
		s_Engine = this;
	}

	Engine::~Engine()
	{
		s_Engine = nullptr;
	}

	void Engine::Initialize()
	{
		DE_LOG(LogEngine, Trace, "Engine Starting up")

		PreInitialize();

		m_WindowSubSystem = MakeRef<WindowSubsystem>();
		m_Subsystems["Window"] = m_WindowSubSystem;

		m_ShaderSubSystem = MakeRef<ShaderSubsystem>();
		m_Subsystems["Shader"] = m_ShaderSubSystem;

		m_RendererSubSystem = MakeRef<RendererSubsystem>();
		m_Subsystems["Renderer"] = m_RendererSubSystem;

		m_UISubSytem = MakeRef<UISubsystem>();
		m_Subsystems["UI"] = m_UISubSytem;

		m_EditorSubSystem = MakeRef<EditorSubsystem>();
		m_Subsystems["Editor"] = m_EditorSubSystem;

		m_SceneSubSystem = MakeRef<SceneSubsystem>();
		m_Subsystems["Scene"] = m_SceneSubSystem;

		m_PhysicsSubSystem = MakeRef<PhysicsSubsystem>();
		m_Subsystems["Physics"] = m_PhysicsSubSystem;

		m_InputSubsystem = MakeRef<InputSubsystem>();
		m_Subsystems["Input"] = m_InputSubsystem;

		// Order of initialization is defined above
		for(const auto& subsystem : m_Subsystems | std::views::values)
			subsystem->Initialize();

		DE_LOG(LogEngine, Info, "Engine Initialized")

		PostInitialize();
	}

	void Engine::Deinitialize()
	{
		DE_LOG(LogEngine, Trace, "Engine Shutting Down")

		// Deinitialie SubSystems in the reverse order of initialization
		for (const auto& subSystem : std::views::reverse(m_Subsystems) | std::views::values)
			subSystem->Deinitialize();

		DE_LOG(LogEngine, Trace, "Engine Deinitialized")
	}

	void Engine::Run()
	{
		Initialize();

		while(m_WindowSubSystem->m_Window->IsOpen())
		{
			const float deltaTime = 0.3f;
			m_InputSubsystem->Poll();

			m_WindowSubSystem->m_Window->ClearBuffer();

			m_EditorSubSystem->Update(deltaTime);

			m_SceneSubSystem->Update(deltaTime);
			
			m_PhysicsSubSystem->Update(deltaTime);
			
			m_WindowSubSystem->m_Window->SwapBuffers();

			m_PhysicsSubSystem->LateUpdate(deltaTime);
			m_SceneSubSystem->CleanRubbish();
		}

		Deinitialize();
	}

	void Engine::PreInitialize()
	{
		DE_LOG(LogEngine, Trace, "Engine Pre-Initialized")
	}

	void Engine::PostInitialize()
	{
		DE_LOG(LogEngine, Trace, "Engine Post-Initialized")
	}
}
