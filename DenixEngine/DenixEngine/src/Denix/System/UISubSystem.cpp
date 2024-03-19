#include "UISubsystem.h"

#include <SDL_video.h>

#include "WindowSubsystem.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

namespace Denix
{
	UISubsystem* UISubsystem::s_UISubSystem{ nullptr };


	UISubsystem::UISubsystem()
	{
		s_UISubSystem = this;

		DE_LOG_CREATE(LogUISubSystem)
	}

	UISubsystem::~UISubsystem()
	{
		s_UISubSystem = nullptr;
	}


	void UISubsystem::Initialize()
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
		if(const WindowSubsystem* windowSystem = WindowSubsystem::Get())
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

	void UISubsystem::Deinitialize()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		m_Initialized = false;
	}
}
