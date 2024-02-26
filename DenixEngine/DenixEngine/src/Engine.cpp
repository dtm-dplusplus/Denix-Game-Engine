#include "DePch.h"
#include "Engine.h"

#include <SDL.h>
#include <GL/glew.h>

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "Video/Window.h"
#include "System/SceneSubsystem.h"
#include "System/WindowSubsystem.h"



Engine::Engine() : m_Running{false}
{
}

Engine::~Engine()
{
}


void Engine::Initialize()
{
	DeLog::Start();
	DE_LOG(Log, Trace, "Engine Starting")

	m_WindowSubSystem = std::make_shared<WindowSubSystem>();
	m_WindowSubSystem->Initialize();
	if (!m_WindowSubSystem->m_Initialized) throw std::exception();
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(m_WindowSubSystem->m_Window->m_SDL_Window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init(m_WindowSubSystem->m_GLSLVersion.c_str());

	// Scene Subsystem
	m_SceneSubSystem = std::make_shared<SceneSubSystem>();
	m_SceneSubSystem->Initialize();
	if(!m_SceneSubSystem->m_Initialized) throw std::exception();

	m_Running = true;

	DE_LOG(Log, Info, "Engine Started")
}

void Engine::Deinitialize()
{
	DE_LOG(Log, Trace, "Engine Stopping")

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	m_SceneSubSystem->Deinitialize();
	m_WindowSubSystem->Deinitialize();

	DE_LOG(Log, Trace, "Engine Stopped")

	DeLog::Stop();
}

void Engine::Run()
{
	Initialize();

	if(!m_Running) throw std::exception();

	

	while(m_WindowSubSystem->GetWindow()->m_IsOpen)
	{
		m_WindowSubSystem->GetWindow()->PollEvents();

		m_WindowSubSystem->GetWindow()->ClearBuffer();

		m_SceneSubSystem->m_ActiveScene->Update();

		m_SceneSubSystem->m_ActiveScene->Draw();

		m_WindowSubSystem->GetWindow()->SwapBuffers();
	}

	m_Running = false;
	Deinitialize();
}