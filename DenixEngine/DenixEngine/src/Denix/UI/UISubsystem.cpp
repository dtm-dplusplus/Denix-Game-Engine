#include "UISubsystem.h"

#include <SDL_video.h>

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "Denix/Video/Window/Window.h"
#include "Denix/Video/Window/WindowSubsystem.h"

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
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Disable Ini file - stops window positions being saved
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
	void UISubsystem::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}
	void UISubsystem::RenderUI()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void UISubsystem::ViewportUpdate(const Ref<SDL_GLWindow> _window)
	{
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			SDL_GL_MakeCurrent(_window->GetSDLWindow(), _window->GetSDL_GLContext());
		}
	}
}
