#include "DePch.h"
#include "Engine.h"

#include "System/WindowSubSystem.h"
#include "System/ShaderSubSystem.h"
#include "System/SceneSubSystem.h"
#include "System/UISubSystem.h"

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
	DeLog::Start();
	DE_LOG(Log, Trace, "Engine Starting up")

	m_WindowSubSystem = std::make_shared<WindowSubSystem>();
	m_WindowSubSystem->Initialize();

	m_ShaderSubSystem = std::make_shared<ShaderSubSystem>();
	m_ShaderSubSystem->Initialize();

	m_UISubSytem = std::make_shared<UISubSystem>();
	m_UISubSytem->Initialize();

	m_SceneSubSystem = std::make_shared<SceneSubSystem>();
	m_SceneSubSystem->Initialize();

	DE_LOG(LogEngine, Info, "Engine Initialized")
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Engine::Deinitialize()
{
	DE_LOG(LogEngine, Trace, "Engine shutting down")

	m_SceneSubSystem->Deinitialize();
	m_UISubSytem->Deinitialize();
	m_ShaderSubSystem->Deinitialize();
	m_WindowSubSystem->Deinitialize();

	DE_LOG(LogEngine, Info, "Engine Deinitialized")

	DeLog::Stop();
}

void Engine::Run()
{
	Initialize();

	while(m_WindowSubSystem->m_Window->m_IsOpen)
	{
		m_WindowSubSystem->m_Window->PollEvents();

		m_WindowSubSystem->m_Window->ClearBuffer();

		m_SceneSubSystem->Update(0.3f);

		m_WindowSubSystem->m_Window->SwapBuffers();

		SDL_Delay(5); // Delay for physics
	}

	Deinitialize();
}