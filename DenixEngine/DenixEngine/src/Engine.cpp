#include "DePch.h"
#include "Engine.h"

#include "System/UISubSystem.h"

Engine* Engine::s_Engine{nullptr};

Engine::Engine() : m_Running{false}
{
}

Engine::~Engine()
{
}


void Engine::Initialize()
{
	DeLog::Start();
	DE_LOG(Log, Trace, "Engine Starting up")

	//  Set Global Engine Pointer
	s_Engine = this;

	// Window Subsystem
	m_WindowSubSystem = std::make_shared<WindowSubSystem>();
	m_WindowSubSystem->Initialize();
	m_EngineWindow = m_WindowSubSystem->m_Window;
	if (!m_WindowSubSystem->m_Initialized) throw std::exception();
	
	// UI Subsystem
	m_UISubSytem = std::make_shared<UISubSystem>();
	m_UISubSytem->Initialize();
	if (!m_UISubSytem->m_Initialized) throw std::exception();

	// Scene Subsystem
	m_SceneSubSystem = std::make_shared<SceneSubSystem>();
	m_SceneSubSystem->Initialize();
	m_EngineScene = m_SceneSubSystem->m_ActiveScene;
	if(!m_SceneSubSystem->m_Initialized) throw std::exception();

	m_Running = true;

	DE_LOG(LogEngine, Info, "Engine Initialized")
}

void Engine::Deinitialize()
{
	DE_LOG(LogEngine, Trace, "Engine shutting down")
	
	m_SceneSubSystem->Deinitialize();
	m_UISubSytem->Deinitialize();
	m_WindowSubSystem->Deinitialize();

	DE_LOG(LogEngine, Info, "Engine Deinitialized")

	DeLog::Stop();
}

void Engine::Run()
{
	Initialize();

	while(m_EngineWindow->m_IsOpen)
	{
		m_EngineWindow->PollEvents();

		m_EngineWindow->ClearBuffer();

		m_EngineScene->Update();

		m_EngineScene->Draw();

		m_EngineWindow->SwapBuffers();
	}

	m_Running = false;
	Deinitialize();
}