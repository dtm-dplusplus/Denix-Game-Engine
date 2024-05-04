#include "depch.h"
#include "Engine.h"


#include "Denix/Video/Window/WindowSubsystem.h"
#include "Denix/UI/UISubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Video/Renderer/RendererSubsystem.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Core/FileSubsystem.h"
#include "Denix/Core/TimerSubsystem.h"

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

		m_TimerSubSystem = MakeRef<TimerSubsystem>();
		m_SubsystemOrder.push_back(m_TimerSubSystem);
		m_Subsystems["Timer"] = m_TimerSubSystem;

		m_FileSubSystem = MakeRef<FileSubsystem>();
		m_FileSubSystem->m_ProjectName = m_ProjectName;
		m_SubsystemOrder.push_back(m_FileSubSystem);
		m_Subsystems["File"] = m_FileSubSystem;

		m_WindowSubSystem = MakeRef<WindowSubsystem>();
		m_SubsystemOrder.push_back(m_WindowSubSystem);
		m_Subsystems["Window"] = m_WindowSubSystem;

		m_RendererSubSystem = MakeRef<RendererSubsystem>();
		m_SubsystemOrder.push_back(m_RendererSubSystem);
		m_Subsystems["Renderer"] = m_RendererSubSystem;

		m_UISubSytem = MakeRef<UISubsystem>();
		m_SubsystemOrder.push_back(m_UISubSytem);
		m_Subsystems["UI"] = m_UISubSytem;

		m_EditorSubSystem = MakeRef<EditorSubsystem>();
		m_SubsystemOrder.push_back(m_EditorSubSystem);
		m_Subsystems["Editor"] = m_EditorSubSystem;

		m_SceneSubSystem = MakeRef<SceneSubsystem>();
		m_SubsystemOrder.push_back(m_SceneSubSystem);
		m_Subsystems["Scene"] = m_SceneSubSystem;

		m_PhysicsSubSystem = MakeRef<PhysicsSubsystem>();
		m_SubsystemOrder.push_back(m_PhysicsSubSystem);
		m_Subsystems["Physics"] = m_PhysicsSubSystem;

		m_InputSubsystem = MakeRef<InputSubsystem>();
		m_SubsystemOrder.push_back(m_InputSubsystem);
		m_Subsystems["Input"] = m_InputSubsystem;

		m_ResourceSubSystem = MakeRef<ResourceSubsystem>();
		m_SubsystemOrder.push_back(m_ResourceSubSystem);
		m_Subsystems["Resource"] = m_ResourceSubSystem;

	    // Order of initialization is defined above
		for(const auto& subsystem : m_SubsystemOrder)
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
			float deltaTime = m_TimerSubSystem->m_DeltaTime;

			m_TimerSubSystem->BeginFrame();

			m_InputSubsystem->Poll();

			m_WindowSubSystem->m_Window->ClearBuffer();

			m_EditorSubSystem->Update(deltaTime);

			m_PhysicsSubSystem->BeginUpdate(deltaTime);

			m_SceneSubSystem->Update(deltaTime);
			
			m_PhysicsSubSystem->Update(deltaTime);
			
			m_WindowSubSystem->m_Window->SwapBuffers();

			m_PhysicsSubSystem->LateUpdate(deltaTime);
			m_SceneSubSystem->CleanRubbish();

			m_TimerSubSystem->EndFrame();
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
