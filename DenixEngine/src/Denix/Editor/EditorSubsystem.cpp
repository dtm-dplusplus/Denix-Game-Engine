#include "EditorSubsystem.h"

#include "Denix.h"
#include "Denix/Editor/Widget/Scene/GameObjectDetailsWidget.h"
#include "Denix/Editor/Widget/Scene/SceneOrganizerWidget.h"
#include "Denix/Editor/Widget/AssetBrowserWidget.h"

namespace Denix
{
	EditorSubsystem* EditorSubsystem::s_EditorSubsystem{ nullptr };

	void EditorSubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogEditor, Warn, "Initializing Editor Subsystem")
		// Get Engine Subsystems
		s_WindowSubsystem = WindowSubsystem::Get();
		s_SceneSubsystem = SceneSubsystem::Get();
		s_InputSubsystem = InputSubsystem::Get();
		s_UISubsystem = UISubsystem::Get();
		m_ActiveScene =s_SceneSubsystem->GetActiveScene();

		// Init Editor Widgets
		m_SceneOrganizerWidget = MakeRef<SceneOrganizerWidget>(m_ActiveScene);
		m_GameObjectDetailsWidget = MakeRef<GameObjectDetailsWidget>();
		m_AssetBrowserWidget = MakeRef<AssetBrowserWidget>();
		DE_LOG(LogEditor, Info, "Editor Subsystem Initialized")
	}

	void EditorSubsystem::Deinitialize()
	{
		DE_LOG(LogEditor, Trace, "Editor Subsystem Initialized")
	}

	void EditorSubsystem::Update(float _deltaTime)
	{
		//if(InputSubsystem::IsKeyDown(SDL_SCANCODE_H)) m_Enabled = !m_Enabled;
		if(!m_Enabled) return;

		EditorWidget::m_DragSpeed = DragSpeed * _deltaTime;
		DragSpeedDelta = EditorWidget::m_DragSpeed;
		
		const glm::vec2 winSize = s_WindowSubsystem->GetWindow()->GetWindowSize();
		WinX = winSize.x;
		WinY = winSize.y;

		MenuBar();
		if (!m_ActiveScene) return;
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_F5))
		{
			if(m_ActiveScene->IsPlaying()) s_SceneSubsystem->StopScene();
            else s_SceneSubsystem->PlayScene();
		}
		
		if(m_AssetBrowserWidget)
		{
			m_AssetBrowserWidget->Update(_deltaTime);
			// TEMP - Remove when AssetBrowser is set to close
			if(m_AssetBrowserWidget->IsRubbish()) m_AssetBrowserWidget.reset();
		}

		SceneWidgets();
		if(m_IsTimerSettingsOpen) TimerSettings();
		if(m_IsInputPanelOpen) s_InputSubsystem->InputPanel();
		if (m_IsPhysicsSettingsOpen) PhysicsSettings();
		if (m_IsProfilerOpen) Profiler();
	}
	
	void EditorSubsystem::MenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("Open Scene", "Alt+F4"))
				{ 
					for (const auto& levelAsset : ResourceSubsystem::GetSceneStore())
					{
						if (ImGui::MenuItem(levelAsset->GetAssetName().c_str()))
						{
							s_SceneSubsystem->OpenScene(levelAsset);
							break;
						}
					}
					ImGui::EndMenu();
				}

				if (ImGui::Button("Save Scene"))
				{
					SceneSubsystem::SerializeScene(m_ActiveScene.get());

					// Temp method to save any changes to materials
					for (const auto& mat : ResourceSubsystem::GetMaterialStore())
					{
						// Save Changes to asset - This should be done in the editor
						YAML::Emitter matAsssetEmitter;
						matAsssetEmitter << YAML::Comment("DE_ASSET: Material");
						matAsssetEmitter << YAML::BeginMap;
						mat.second->Serialize(matAsssetEmitter);
						matAsssetEmitter << YAML::EndMap;

						FileSubsystem::WriteFile(mat.second->GetAsset()->GetAssetPath(), matAsssetEmitter.c_str());
						DE_LOG(LogScene, Info, "Serialized Material");
					}
				}
				
				if (ImGui::MenuItem("Quit", "Alt+F4")) 
				{
					s_WindowSubsystem->GetWindow()->RequestClose();
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
					Engine::Get()->SetStartupScene(m_ActiveScene->m_SceneAsset);
				}
				ImGui::EndMenu();
			}
				
			if (ImGui::BeginMenu("Window"))
			{
				if(ImGui::MenuItem("AssetBrowser", nullptr))
				{
					if(!m_AssetBrowserWidget) m_AssetBrowserWidget = MakeRef<AssetBrowserWidget>();
				}
				ImGui::Checkbox("Timer Settings", &m_IsTimerSettingsOpen);
				ImGui::Checkbox("Profiler", &m_IsProfilerOpen);
				ImGui::Checkbox("Physics Settings", &m_IsPhysicsSettingsOpen);
				ImGui::Checkbox("Input Debugger", &m_IsInputPanelOpen);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Toggle Fullscreen", "F11"))
				{
					s_WindowSubsystem->GetWindow()->ToggleFullscreen();
				}

				ImGui::EndMenu();
			}

			

			if (!m_ActiveScene->IsPlaying())
			{
				if (ImGui::Button("Play"))
				{
					s_SceneSubsystem->PlayScene();
				}
			}
			else
			{
				if (ImGui::Button("Pause"))
				{
					s_SceneSubsystem->PauseScene();
				}
				ImGui::SameLine();
				if (ImGui::Button("Stop"))
				{
					if(m_SceneOrganizerWidget) m_SceneOrganizerWidget->ResetSelection();
					s_SceneSubsystem->StopScene();
				}
			}

			// Scene Properties
			if (ImGui::BeginMenu("Tools"))
			{
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
					
				ImGui::DragFloat("UI Drag Speed", &DragSpeed, DragSpeed, 0.1f, 10.0f);

				ImGui::ColorEdit4("Clear Color", &s_WindowSubsystem->GetWindow()->GetClearColor()[0]);
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
	
	void EditorSubsystem::SetActiveScene(const Ref<Scene>& _scene)
	{
		m_ActiveScene = _scene;
		if(m_SceneOrganizerWidget)
		{
			m_SceneOrganizerWidget->m_SceneRef = _scene;
			m_SceneOrganizerWidget->ResetSelection();
		}
		if (m_GameObjectDetailsWidget) m_GameObjectDetailsWidget->m_GameObjectRef.reset();
	}

	void EditorSubsystem::PhysicsSettings()
	{
		ImGui::SetNextWindowSize(ImVec2((WinX / 5), WinY), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2((WinX / 2), WinY / 2), ImGuiCond_Appearing);

		if (ImGui::Begin("Physics Settings", &m_IsPhysicsSettingsOpen))
		{
			ImGui::Checkbox("Collision Detection", &PhysicsSubsystem::CollisionDetectionEnabledRef());
			ImGui::Checkbox("Collision Response", &PhysicsSubsystem::CollisionResponseEnabledRef());
			ImGui::End();
		}
	}

	void EditorSubsystem::TimerSettings()
	{
		ImGui::DragInt("Max FPS", &TimerSubsystem::GetMaxFPS(), 1, 0, 240);
		ImGui::SliderFloat("Game Speed", &TimerSubsystem::GetGameTimeSpeed(), 0.0f, 2.0f);
		ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTimeMs());
		ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());
	}

	void EditorSubsystem::Profiler()
	{
		ImGui::Begin("Profiler", &m_IsProfilerOpen);
		{
			if (ImGui::CollapsingHeader("Thread", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
			{
				ImGui::Text("Thread Count: %d", std::thread::hardware_concurrency());
				ImGui::SeparatorText("Scene Thread");
				ImGui::Checkbox("Scene Threaded", &SceneSubsystem::Get()->m_SceneThreaded);
			}

			const auto& profiles = ProfileSubsystem::Get()->GetProfiles();
			const float elaspedTime = TimerSubsystem::GetProgramElapsedTime();

			ImGui::DragInt("Max FPS", &TimerSubsystem::GetMaxFPS(), 1, 0, 240);
			ImGui::Text("Program time: %fms", TimerSubsystem::GetProgramElapsedTime());
			ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTime());
			ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());


			static float history = 5.0f;
			ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

			const Ref<Profile>& engprofile = TimerSubsystem::Get()->m_EngineProfile;

			if (ImPlot::BeginPlot("Profile Visualizer", nullptr, "Frame Time (ms)", ImVec2(-1, 0), ImPlotFlags_None,
								  ImPlotFlags_None, ImPlotAxisFlags_AutoFit))
			{
				ImPlot::SetupAxisLimits(ImAxis_X1, elaspedTime - history, elaspedTime, ImGuiCond_Always);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, engprofile->m_AverageDuration * 1.5f, ImGuiCond_Always);
				ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
				for (const auto& [name, profile] : profiles)
				{
					if (profile->m_Buffer.Data.size() > 0)
						ImPlot::PlotLine(name.c_str(), &profile->m_Buffer.Data[0].x, &profile->m_Buffer.Data[0].y,
										 profile->m_Buffer.Data.size(), 0, profile->m_Buffer.Offset, 2 * sizeof(float));
				}
				ImPlot::EndPlot();
			}

			for (const auto& [name, profile] : profiles)
			{
				if (ImGui::TreeNode(name.c_str()))
				{
					if (ImPlot::BeginPlot("##Profiling", nullptr, "Frame Time (ms)",ImVec2(-1, 0), ImPlotFlags_None, ImPlotFlags_None, ImPlotAxisFlags_AutoFit))
					{
						ImPlot::SetupAxisLimits(ImAxis_X1,elaspedTime - history, elaspedTime, ImGuiCond_Always);
						ImPlot::SetupAxisLimits(ImAxis_Y1,profile->m_AverageDuration * 0.5f,profile->m_MaximumDuration * 1.25f, ImGuiCond_Always);
						ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
						ImPlot::PlotLine(name.c_str(), &profile->m_Buffer.Data[0].x, &profile->m_Buffer.Data[0].y, profile->m_Buffer.Data.size(), 0, profile->m_Buffer.Offset, 2*sizeof(float));
						ImPlot::EndPlot();
					}
					ImGui::Text("Frame Percentage: %.2f%%", profile->m_FramePercentage * 100.0f);
					ImGui::Text("Duration: %fms", profile->GetDuration());
					ImGui::Text("Minimum Duration: %fms", profile->m_MinimumDuration);
					ImGui::Text("Maximum Duration: %fms", profile->m_MaximumDuration);
					ImGui::Text("Average Duration: %fms", profile->m_AverageDuration);
					if (ImGui::DragInt("Average Duration Count", &profile->m_AverageDurationCount, 1.0f, 3, 100))
					{
						profile->m_DurationRecords.resize(profile->m_AverageDurationCount);
					}
					ImGui::TreePop();
				}
			}
			ImGui::End();
		}
	}

	void EditorSubsystem::SceneWidgets()
	{
		//ImGui::SetNextWindowSize(ImVec2((WinX / 6), WinY), ImGuiCond_Appearing);
		//ImGui::SetNextWindowPos(ImVec2(0, MenuBarHeight), ImGuiCond_Appearing); // + ViewportBarHeight
		//ImGui::SetNextItemOpen(&ScenePanelOpen, ImGuiCond_Appearing);

		//ImGui::SetNextWindowDockID(s_UISubsystem->DockLeftID, ImGuiCond_FirstUseEver);
		//ImGui::Begin("Scene Panel", &ScenePanelOpen);
		//ScenePropertiesWidget();
		//SceneAddObjectWidget();
		if(m_SceneOrganizerWidget) m_SceneOrganizerWidget->Update(0.0f);
		if (m_GameObjectDetailsWidget)
		{
			m_GameObjectDetailsWidget->m_GameObjectRef = m_SceneOrganizerWidget->GetSelectedObject();
			m_GameObjectDetailsWidget->Update(0.0f);
		}
		//ImGui::End();
	}

	/////////////// WIDGETS //////////////////////////////
	void EditorSubsystem::ScenePropertiesWidget() const
	{
		ImGui::SeparatorText("Scene Properties");

		// Viewport Mode
		ImGui::Combo("Viewport Mode", &RendererSubsystem::GetViewportMode(), "Default\0Unlit\0Wireframe\0Collision\0\0");
		
		//CameraWidget(m_ActiveScene->m_ViewportCamera);

		if (ImGui::TreeNode("Scene Settings"))
		{
			// Scene gravity
			ImGui::DragFloat("Scene Gravity", &s_SceneSubsystem->m_ActiveScene->GetGravity(), DragSpeedDelta, -FLT_MAX, FLT_MAX);

			// Viewport Camera Properties

			ImGui::TreePop();
		}
	}
	
}
