#include "DePch.h"
#include "Engine.h"

#include <SDL.h>
#include <GL/glew.h>

#include "Video/Window.h"
#include "Scene/ExampleScenes.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"



Engine::Engine()
{
}


bool Engine::Start()
{
	DeLog::Start();
	DE_LOG(Log, Trace, "Engine Starting")

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0)
	{
		DE_LOG(Log, Critical, "SDL Init failed! SDL_Error: {}", SDL_GetError())
		return false;
	}
	DE_LOG(Log, Trace, "SDL Init success")

	// Set SDL OpenGL Version
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Useful for virtual keyboards, complex chararacter sets
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	// Set GL Attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//Create window
	SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	m_Window = std::make_shared<Window>();
	if(!m_Window->Start(flags))
		return false;

	// Init Glew
	if (glewInit() != GLEW_OK)
	{
		DE_LOG(Log, Critical, "glewInit failed!")
			return false;
	}
	DE_LOG(Log, Trace, "glewInit success")

	// Enable Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(m_Window->m_SDL_Window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init(glsl_version);

	DE_LOG(Log, Info, "Engine Started")

	return true;
}

void Engine::Stop()
{
	DE_LOG(Log, Trace, "Engine Stopping")

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	m_Window->Stop();

	SDL_Quit();

	DE_LOG(Log, Trace, "Engine Stopped")

	DeLog::Stop();
}

void Engine::Run()
{
	if(!Start()) throw std::exception();

	// Load Example Scene
	std::unique_ptr<Scene> scene = std::make_unique<ObjectScene>();

	while(m_Window->m_IsOpen)
	{
		m_Window->PollEvents();

		m_Window->ClearBuffer();

		// Update Project Scene here
		scene->Update();
		scene->Draw();

		m_Window->SwapBuffers();
	}

	Stop();
}