#include "UISubsystem.h"

#include <SDL3/SDL_video.h>
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl3.h"

#include "Denix/Video/Window/Window.h"
#include "Denix/Video/Window/WindowSubsystem.h"

namespace Denix
{
	UISubsystem* UISubsystem::s_UISubSystem{ nullptr };


	UISubsystem::UISubsystem()
	{
		s_UISubSystem = this;

		DE_LOG_CREATE(LogUI)
	}

	UISubsystem::~UISubsystem()
	{
		s_UISubSystem = nullptr;
	}


	void UISubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogUI, Warn, "UI Subsystem Initializing")
		
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Disable Ini file - stops window positions being saved
		io.IniFilename = nullptr;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup SDL3 implementation
		
		// Setup Platform/Renderer backends
		const WindowSubsystem* windowSystem = WindowSubsystem::Get();
		if(!ImGui_ImplSDL3_InitForOpenGL(windowSystem->GetWindow()->GetSDLWindow(),
				SDL_GL_GetCurrentContext()))
		{
			throw std::runtime_error("ImGui_ImplSDL3_InitForOpenGL failed");
		}

		if(!ImGui_ImplOpenGL3_Init(windowSystem->GetWindow()->GetGLSLVersion().c_str()))
		{
			throw std::runtime_error("ImGui_ImplOpenGL3_Init failed");
		}
		
		DE_LOG(LogUI, Info, "UI Subsystem Initialized")
	}

	void UISubsystem::Deinitialize()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}
	void UISubsystem::Update(float _deltaTime)
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* imguiViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(imguiViewport->Pos);
		ImGui::SetNextWindowSize(imguiViewport->Size);
		ImGui::SetNextWindowViewport(imguiViewport->ID);
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

			static auto first_time = true;
			if (first_time)
			{
				first_time = false;

				ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
				ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspace_id, imguiViewport->Size);

				// split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
				//   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
				//                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
				DockLeftID = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
				DockRightID = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f, nullptr, &dockspace_id);
				DockDownID = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);

				// we now dock our windows into the docking node we made above
				ImGui::DockBuilderDockWindow("Right", DockRightID);
				ImGui::DockBuilderDockWindow("Left", DockLeftID);
				ImGui::DockBuilderDockWindow("Down", DockDownID);

				ImGui::DockBuilderFinish(dockspace_id);
			}
		}
		ImGui::End();
	}
	void UISubsystem::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
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
