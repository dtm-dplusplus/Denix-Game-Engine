#include "UISubSystem.h"

UISubSystem::UISubSystem()
{}

UISubSystem::~UISubSystem()
{}


void UISubSystem::Initialize()
{
	const Engine& engine = Engine::Get();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
		
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(engine.GetEngineWindow()->GetSDLWindow(), SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init(engine.GetWindowSubSystem()->GetGLSLVersion().c_str());

	m_Initialized = true;
}

void UISubSystem::Deinitialize()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
