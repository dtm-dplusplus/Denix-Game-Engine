#include "EditorSubsystem.h"

#include "Denix.h"
#include "Denix/Editor/Widget/Scene/ActorDetailsWidget.h"
#include "Denix/Editor/Widget/Scene/SceneOrganizerWidget.h"
#include "Denix/Editor/Widget/AssetBrowserWidget.h"
#include "Denix/Editor/Widget/EngineProfilerWidget.h"
#include "Denix/Editor/Widget/InputDebuggerWidget.h"
#include "Denix/Thread/JobSubsystem.h"
#include "Widget/PerformanceSettingsWidget.h"

namespace Denix
{
	EditorSubsystem* EditorSubsystem::s_EditorSubsystem{ nullptr };

	EditorSubsystem::EditorSubsystem()
	{
		s_EditorSubsystem = this;
		DE_LOG_CREATE(LogEditor)
	}
	
	void EditorSubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogEditor, Warn, "Initializing Editor Subsystem")
		m_ActiveScene = SceneSubsystem::GetActiveScene();

		// Init Editor Widgets
		m_SceneOrganizerWidget = MakeRef<SceneOrganizerWidget>(m_ActiveScene);
		m_ActorDetailsWidget = MakeRef<ActorDetailsWidget>();
		m_AssetBrowserWidget = MakeRef<AssetBrowserWidget>();
		DE_LOG(LogEditor, Info, "Editor Subsystem Initialized")
	}

	void EditorSubsystem::Deinitialize()
	{
		DE_LOG(LogEditor, Trace, "Editor Subsystem Initialized")
	}

	void EditorSubsystem::Update(float _deltaTime)
	{
		DE_PROFILE(Editor Update)

		if(!m_Enabled) return;
		if (!m_ActiveScene) return;

		EditorWidget::m_DragSpeed = EditorWidget::m_DragSensitivity * _deltaTime;
		m_DragSpeed = EditorWidget::m_DragSpeed;

		MainMenuBar();

		// Asset Browser
		if(m_AssetBrowserWidget)
		{
			m_AssetBrowserWidget->Update(_deltaTime);
			// TEMP - Remove when AssetBrowser is set to close
			if(m_AssetBrowserWidget->IsRubbish()) m_AssetBrowserWidget.reset();
		}

		// Scene Widgets
		if(m_SceneOrganizerWidget) m_SceneOrganizerWidget->Update(0.0f);
		if (m_ActorDetailsWidget)
		{
			m_ActorDetailsWidget->m_ActorRef = m_SceneOrganizerWidget->GetSelectedObject();
			m_ActorDetailsWidget->Update(0.0f);
		}
		
		if(m_PerformanceSettingsWidget) m_PerformanceSettingsWidget->Update(_deltaTime);
		if(m_InputDebuggerWidget) m_InputDebuggerWidget->Update(_deltaTime);
		if (m_EngineProfilerWidget) m_EngineProfilerWidget->Update(_deltaTime);

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
					for (const auto& levelAsset : ResourceSubsystem::GetSceneStore())
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
					Engine::SetStartupScene(m_ActiveScene->m_SceneAsset);
				}
				ImGui::EndMenu();
			}
				
			if (ImGui::BeginMenu("Window"))
			{
				if(ImGui::MenuItem("AssetBrowser", nullptr))
				{
					if(!m_AssetBrowserWidget) m_AssetBrowserWidget = MakeRef<AssetBrowserWidget>();
				}
				if(ImGui::MenuItem("Performance Settings", nullptr))
				{
					if(!m_PerformanceSettingsWidget) m_PerformanceSettingsWidget = MakeRef<PerformanceSettingsWidget>();
				}
				
				if (ImGui::MenuItem("Profiler", nullptr))
				{
					if(!m_EngineProfilerWidget) m_EngineProfilerWidget = MakeRef<EngineProfilerWidget>();
				}
				if (ImGui::MenuItem("Input Debugger", nullptr))
				{
					if (!m_InputDebuggerWidget) m_InputDebuggerWidget = MakeRef<InputDebuggerWidget>();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Toggle Fullscreen", "F11"))
				{
					WindowSubsystem::ToggleFullscreen();
				}

				ImGui::EndMenu();
			}

			if (!m_ActiveScene->IsPlaying())
			{
				if (ImGui::Button("Play"))
				{
					SceneSubsystem::PlayScene();
				}
			}
			else
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

			// Scene Properties
			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::Checkbox("Scene Threaded", &SceneSubsystem::Get()->m_BatchUpdateActors);
				ImGui::Checkbox("Renderer Enabled", &RendererSubsystem::Get()->IsEnabled());
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
}
