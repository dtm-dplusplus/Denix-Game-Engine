#include "UISubSystem.h"

#include <SDL_video.h>

#include "WindowSubSystem.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

namespace Denix
{
	UISubSystem* UISubSystem::s_UISubSystem{ nullptr };


	UISubSystem::UISubSystem()
	{
		s_UISubSystem = this;

		DE_LOG_CREATE(LogUISubSystem)
	}

	UISubSystem::~UISubSystem()
	{
		s_UISubSystem = nullptr;
	}


	void UISubSystem::Initialize()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		io.IniFilename = nullptr;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		
		// Setup Platform/Renderer backends
		if(const WindowSubSystem* windowSystem = WindowSubSystem::Get())
		{
			// Setup Platform/Renderer bindings
			ImGui_ImplSDL2_InitForOpenGL(windowSystem->GetWindow()->GetSDLWindow(), SDL_GL_GetCurrentContext());
			ImGui_ImplOpenGL3_Init(windowSystem->GetWindow()->GetGLSLVersion().c_str());
		}
		else
		{
			DE_LOG(LogUISubSystem, Critical, "Failed to initialize UI Subsystem")
			return;
		}

		DE_LOG(LogUISubSystem, Trace, "UI SubSystem Initialized")
		m_Initialized = true;
	}

	void UISubSystem::Deinitialize()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		m_Initialized = false;
	}
}
