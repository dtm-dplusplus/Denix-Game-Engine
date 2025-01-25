#include "EditorSubsystem.h"

#include <backends/imgui_impl_opengl3.h>

#include "Denix.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl3.h"
#include "Denix/Core/File/FileSubsystem.h"
#include "Denix/Editor/Widget/Scene/ActorDetailsWidget.h"
#include "Denix/Editor/Widget/Scene/SceneOrganizerWidget.h"
#include "Denix/Editor/Widget/AssetBrowserWidget.h"
#include "Denix/Editor/Widget/Profile/EngineProfilerWidget.h"
#include "Denix/Editor/Widget/InputDebuggerWidget.h"
#include "Denix/Core/Thread/JobSubsystem.h"
#include "Widget/Profile/PerformanceSettingsWidget.h"

#include "yaml-cpp/yaml.h"

namespace Denix
{
	void EditorSubsystem::Initialize()
	{
		DE_LOG(LogEditor, Warn, "Initializing Editor Subsystem")
		Subsystem::Initialize();

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

		// Setup SDL3 Platform/Renderer backends
		m_WindowRef = WindowSubsystem::GetWindow();
		DE_ASSERT(m_WindowRef.lock(), "Window Reference is null")
		
		DE_ASSERT(ImGui_ImplSDL3_InitForOpenGL(m_WindowRef.lock()->GetSDLWindow(),
				SDL_GL_GetCurrentContext()),"ImGui_ImplSDL3_InitForOpenGL failed")

		DE_ASSERT(ImGui_ImplOpenGL3_Init(m_WindowRef.lock()->GetGLSLVersion().c_str()),"ImGui_ImplOpenGL3_Init failed")
	
		m_ActiveScene = SceneSubsystem::GetActiveScene();
		DE_ASSERT(m_ActiveScene.lock(), "Scene ref is null")
		
		// Init Editor Widgets
		m_SceneOrganizerWidget = MakeRef<SceneOrganizerWidget>(m_ActiveScene);
		m_ActorDetailsWidget = MakeRef<ActorDetailsWidget>();
		m_AssetBrowserWidget = MakeRef<AssetBrowserWidget>();
		DE_LOG(LogEditor, Info, "Editor Subsystem Initialized")
	}

	void EditorSubsystem::Deinitialize()
	{
		DE_LOG(LogEditor, Trace, "Editor Subsystem Deinitializing")
		m_ActiveScene.reset();
		m_SceneOrganizerWidget.reset();
		m_ActorDetailsWidget.reset();
		m_AssetBrowserWidget.reset();
		m_PerformanceSettingsWidget.reset();
		m_EngineProfilerWidget.reset();
		m_InputDebuggerWidget.reset();
		
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
		Subsystem::Deinitialize();
		
		DE_LOG(LogEditor, Trace, "Editor Subsystem Deinitialized")
	}

	void EditorSubsystem::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}
	void EditorSubsystem::RenderUI()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void EditorSubsystem::PresentFrame()
	{
		const auto& window = s_Instance->m_WindowRef.lock();
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			SDL_GL_MakeCurrent(window->GetSDLWindow(), window->GetSDL_GLContext());
		}
	}

	void EditorSubsystem::Update(float _deltaTime)
	{
		if(!m_Enabled) return;
		if (!m_ActiveScene.lock()) return;

		DE_PROFILE(Editor Update)
		ViewportUpdate();
		
		EditorWidget::m_DragSpeed = EditorWidget::m_DragSensitivity * _deltaTime;
		m_DragSpeed = EditorWidget::m_DragSpeed;

		MainMenuBar();

		// Scene Widgets
		if(m_SceneOrganizerWidget) m_SceneOrganizerWidget->Update(0.0f);
		if (m_ActorDetailsWidget)
		{
			m_ActorDetailsWidget->m_ActorRef = m_SceneOrganizerWidget->GetSelectedObject();
			m_ActorDetailsWidget->Update(0.0f);
		}
		
		for (const auto& widget : m_EditorWidgets)
		{
			if (widget)
			{
				widget->Update(_deltaTime);
				if (widget->IsRubbish()) RemoveEditorWidget(widget);
			}
		}

		DE_PROFILE_END(Editor Update)
	}
	
	void EditorSubsystem::MainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("Open Scene", "Alt+F4"))
				{ 
					for (const auto& levelAsset : AssetSubsystem::GetSceneStore())
					{
						if (ImGui::MenuItem(levelAsset->GetAssetName().c_str()))
						{
							SceneSubsystem::OpenScene(levelAsset);
							break;
						}
					}
					ImGui::EndMenu();
				}

				if (ImGui::Button("Save Scene"))
				{
					SceneSubsystem::SerializeScene();

					// Temp method to save any changes to materials
					for (const auto& mat : AssetSubsystem::GetMaterialStore())
					{
						// Save Changes to asset - This should be done in the editor
						YAML::Emitter matAsssetEmitter;
						matAsssetEmitter << YAML::Comment("DE_ASSET: Material");
						matAsssetEmitter << YAML::BeginMap;
						mat.second->Serialize(matAsssetEmitter);
						matAsssetEmitter << YAML::EndMap;

						FileSubsystem::WriteFile(mat.second->GetAsset()->GetRelativePath(), matAsssetEmitter.c_str());
						DE_LOG(LogScene, Info, "Serialized Material");
					}
				}
				
				if (ImGui::MenuItem("Quit", "Alt+F4")) 
				{
					WindowSubsystem::GetWindow()->RequestClose();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z", false, false)) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "Ctrl+X", false, false)) {}
				if (ImGui::MenuItem("Copy", "Ctrl+C", false, false)) {}
				if (ImGui::MenuItem("Paste", "Ctrl+V", false, false)) {}

				ImGui::Text("Project Settings");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene"))
			{
				if(ImGui::MenuItem("Set as Startup Scene"))
				{
					AssetSubsystem::SetStartupScene(m_ActiveScene.lock()->m_SceneAsset->GetRelativePath());
				}
				ImGui::EndMenu();
			}
				
			if (ImGui::BeginMenu("Window"))
			{
				if(ImGui::MenuItem("AssetBrowser", nullptr))
					AddEditorWidget<AssetBrowserWidget>();
				
				if(ImGui::MenuItem("Performance Settings", nullptr))
					AddEditorWidget<PerformanceSettingsWidget>();
				
				if (ImGui::MenuItem("Profiler", nullptr))
					AddEditorWidget<EngineProfilerWidget>();
				
				if (ImGui::MenuItem("Input Debugger", nullptr))
					AddEditorWidget<InputDebuggerWidget>();
				
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Toggle Fullscreen", "F11"))
					WindowSubsystem::ToggleFullscreen();

				ImGui::EndMenu();
			}

			switch (SceneSubsystem::GetSceneState())
			{
				case SceneState::Playing:
					{
						if (ImGui::Button("Pause"))
						{
							SceneSubsystem::PauseScene();
						}
						ImGui::SameLine();
						if (ImGui::Button("Stop"))
						{
							if(m_SceneOrganizerWidget) m_SceneOrganizerWidget->ResetSelection();
							SceneSubsystem::StopScene();
						}
					}
					break;

				case SceneState::Stopped:
					{
						if (ImGui::Button("Play"))
						{
							SceneSubsystem::PlayScene();
						}
					}
					break;

				case SceneState::Paused:
					{
						if (ImGui::Button("Resume"))
						{
							SceneSubsystem::PauseScene();
						}
						ImGui::SameLine();
						if (ImGui::Button("Stop"))
						{
							if(m_SceneOrganizerWidget) m_SceneOrganizerWidget->ResetSelection();
							SceneSubsystem::StopScene();
						}
					}
					break;
			}

			// Scene Properties
			if (ImGui::BeginMenu("Tools"))
			{
				if(ImGui::Checkbox("Keyboard Logging", &Keyboard::m_KeyboardLogging)) Engine::GetConfig().KeyboardLogging = Keyboard::m_KeyboardLogging;
				if (ImGui::Checkbox("Mouse Button Logging", &Mouse::m_MouseButtonLogging)) Engine::GetConfig().MouseButtonLogging = Mouse::m_MouseButtonLogging;
				if (ImGui::Checkbox("Mouse Motion Logging", &Mouse::m_MouseMotionLogging)) Engine::GetConfig().MouseMotionLogging = Mouse::m_MouseMotionLogging;
				ImGui::Checkbox("Scene Threaded", &SceneSubsystem::GetInstance()->m_BatchUpdateActors);
				ImGui::Checkbox("Renderer Enabled", &RendererSubsystem::GetInstance()->IsEnabled());
				if(ImGui::BeginMenu("Reflection"))
				{
					for (const auto& key : ReflectionSubsystem::GetCreateFuncs() | std::views::keys)
					{
						ImGui::MenuItem(key.c_str());
					}
					ImGui::EndMenu();
				}
				
				ImGui::Checkbox("Show Demo Window", &ShowDemoWindow);
				if (ShowDemoWindow) ImGui::ShowDemoWindow(&ShowDemoWindow);

				ImGui::Checkbox("Show Plot Demo Window", &ShowPlotDemoWindow);
				if (ShowPlotDemoWindow) ImPlot::ShowDemoWindow(&ShowPlotDemoWindow);
					
				ImGui::DragFloat("UI Drag Sensitivity", &EditorWidget::m_DragSensitivity, m_DragSpeed, 0.1f, 10.0f);

				ImGui::ColorEdit4("Clear Color", &WindowSubsystem::GetWindow()->GetClearColor()[0]);
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void EditorSubsystem::SetActiveScene(const Ref<Scene>& _scene)
	{
		m_ActiveScene = _scene;
		if(m_SceneOrganizerWidget) m_SceneOrganizerWidget->SceneChangedEvent(_scene);
		if (m_ActorDetailsWidget) m_ActorDetailsWidget->m_ActorRef.reset();
	}

		void EditorSubsystem::ViewportUpdate()
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
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
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
}
