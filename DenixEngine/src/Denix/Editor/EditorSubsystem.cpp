#include "EditorSubsystem.h"

#include "Denix.h"
#include "Widget/SceneOrganizerWidget.h"

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
		s_RendererSubsystem = RendererSubsystem::Get();
		s_UISubsystem = UISubsystem::Get();
		m_ActiveScene =s_SceneSubsystem->GetActiveScene();

		// Init Editor Widgets
		m_SceneOrganizerWidget = MakeRef<SceneOrganizerWidget>(m_ActiveScene);
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

		DragSpeedDelta = DragSpeed * _deltaTime;
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
		if(m_IsScenePanelOpen) ScenePanel();
		if (m_IsDetailsPanelOpen) DetailsPanel();
		if(m_IsTimerSettingsOpen) TimerSettings();
		if(m_IsInputPanelOpen) s_InputSubsystem->InputPanel();
		if (m_IsPhysicsSettingsOpen) PhysicsSettings();
		if (m_IsAssetBrowserOpen) AssetBrowser();
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
				ImGui::SeparatorText("Panels");
				ImGui::Checkbox("Scene Panel", &m_IsScenePanelOpen);
				ImGui::Checkbox("Details Panel", &m_IsDetailsPanelOpen);
				ImGui::Checkbox("Timer Settings", &m_IsTimerSettingsOpen);
				ImGui::Checkbox("Profiler", &m_IsProfilerOpen);
				ImGui::Checkbox("Physics Settings", &m_IsPhysicsSettingsOpen);
				ImGui::Checkbox("Input Debugger", &m_IsInputPanelOpen);
				ImGui::Checkbox("Asset Browser", &m_IsAssetBrowserOpen);
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
					m_ObjectSelection = -1;
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



	void EditorSubsystem::AssetBrowser()
	{
		if (ImGui::CollapsingHeader("Assets"))
        {
			ImGui::Text("Project Name: %s", FileSubsystem::GetProjectName().c_str());
			ImGui::Text("Project Root: %s", FileSubsystem::GetProjectRoot().c_str());
			ImGui::Text("User Content Root: %s", FileSubsystem::GetContentRoot().c_str());
			
            if (ImGui::TreeNode("Materials"))
            {
                for (const auto& mat : ResourceSubsystem::GetMaterialStore())
                {
                    ImGui::Text(mat.second->GetAsset()->GetAssetName().c_str());
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Scenes"))
            {
                
                for (const auto& scene : ResourceSubsystem::GetSceneStore())
                {
                    ImGui::Text(scene->GetAssetName().c_str());
                    ImGui::Text("Asset Path: %s", scene->GetAssetPath().c_str());
                }
                ImGui::TreePop();
            }
        }
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

	void EditorSubsystem::ScenePanel()
	{
		ImGui::SetNextWindowSize(ImVec2((WinX / 6), WinY), ImGuiCond_Appearing);
		//ImGui::SetNextWindowPos(ImVec2(0, MenuBarHeight), ImGuiCond_Appearing); // + ViewportBarHeight
		ImGui::SetNextItemOpen(&ScenePanelOpen, ImGuiCond_Appearing);

		ImGui::SetNextWindowDockID(s_UISubsystem->DockLeftID, ImGuiCond_FirstUseEver);
		ImGui::Begin("Scene Panel", &ScenePanelOpen);
		ScenePropertiesWidget();
		SceneAddObjectWidget();
		if(m_SceneOrganizerWidget) m_SceneOrganizerWidget->Update(0.0f);
		ImGui::End();
	}

	void EditorSubsystem::DetailsPanel()
	{
		ImGui::SetNextWindowSize(ImVec2((WinX / 5), WinY), ImGuiCond_Appearing);
		//ImGui::SetNextWindowPos(ImVec2((WinX / 6), MenuBarHeight), ImGuiCond_Appearing);

		ImGui::SetNextWindowDockID(s_UISubsystem->DockRightID);
		if (ImGui::Begin("Details Panel", &ScenePanelOpen))
		{
			if (m_ObjectSelection >= 0 && m_ObjectSelection < m_ActiveScene->m_SceneObjects.size())
			{
				const Ref<GameObject> selectedObject = m_ActiveScene->m_SceneObjects[m_ObjectSelection];

				ImGui::SeparatorText(selectedObject->GetName().c_str());

				TransformWidget(selectedObject);
				CameraWidget(selectedObject);
				LightWidget(selectedObject);
				PhysicsWidget(selectedObject);
				CollisionWidget(selectedObject);
				RenderWidget(selectedObject);
				MeshWidget(selectedObject);
			}
			ImGui::End();
		}
	}

	/////////////// WIDGETS //////////////////////////////
	void EditorSubsystem::ScenePropertiesWidget() const
	{
		ImGui::SeparatorText("Scene Properties");

		// Viewport Mode
		ImGui::Combo("Viewport Mode", &RendererSubsystem::GetViewportMode(), "Default\0Unlit\0Wireframe\0Collision\0\0");
		
		CameraWidget(m_ActiveScene->m_ViewportCamera);

		if (ImGui::TreeNode("Scene Settings"))
		{
			// Scene gravity
			ImGui::DragFloat("Scene Gravity", &s_SceneSubsystem->m_ActiveScene->GetGravity(), DragSpeedDelta, -FLT_MAX, FLT_MAX);

			// Viewport Camera Properties

			ImGui::TreePop();
		}
	}

	void EditorSubsystem::SceneAddObjectWidget()
	{
		ImGui::Separator();
		ImGui::BeginChild("AddObject", ImVec2(100, 25));
		bool createdObject = false;

		const static std::string shapeNames[] = { "Plane", "Cube", "Sphere"};
		const static std::string lightNames[] = { "Directional Light", "Point Light", "Spot Light" };

		if (ImGui::Button("Add"))
			ImGui::OpenPopup("add_object_popup");

		if (ImGui::BeginPopup("add_object_popup"))
		{
			if (ImGui::BeginMenu("Shapes"))
			{
				for (auto& name : shapeNames)
				{
					if (ImGui::MenuItem(name.c_str()))
					{
						createdObject = true;
						if (name == "Plane")
						{
							m_ActiveScene->SpawnGameObject<Plane>();
						}
						else if (name == "Cube")
						{
							m_ActiveScene->SpawnGameObject<Cube>();
						}
						else if (name == "Sphere")
						{
							m_ActiveScene->SpawnGameObject<Sphere>();
						}
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Lights"))
			{
				for (auto& name : lightNames)
				{
					if (ImGui::MenuItem(name.c_str()))
					{
						createdObject = true;
						if (name == lightNames[0])
						{
							// Check if the scene already has a directional light
							if (m_ActiveScene->GetDirectionalLight())
							{
								DE_LOG(LogEditor, Warn, "Scene already has a directional light")
								break;
							}
							const Ref<DirectionalLight> dirLight = MakeRef<DirectionalLight>();
							dirLight->BeginScene();
							m_ActiveScene->m_DirLight = dirLight;
							m_ActiveScene->m_SceneObjects.push_back(dirLight);
							DE_LOG(LogEditor, Info, "Added {} to the scene", lightNames[0])
						}
						else if (name == lightNames[1])
						{
							const Ref<PointLight> pointLight = MakeRef<PointLight>();
							pointLight->BeginScene();
							m_ActiveScene->m_PointLights.push_back(pointLight);
							m_ActiveScene->m_SceneObjects.push_back(pointLight);
							DE_LOG(LogEditor, Info, "Added {} to the scene", lightNames[1])
						}
						else if (name == lightNames[2])
						{
							const Ref<SpotLight> spotLight = MakeRef<SpotLight>();
							spotLight->BeginScene();
							m_ActiveScene->m_SpotLights.push_back(spotLight);
							m_ActiveScene->m_SceneObjects.push_back(spotLight);
							DE_LOG(LogEditor, Info, "Added {} to the scene", lightNames[2])
						}
					}
				}
				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		if (createdObject) m_SceneOrganizerWidget->SetObjectSelection(m_ActiveScene->m_SceneObjects.size() - 1);
		ImGui::EndChild();
		ImGui::Separator();
	}

	void EditorSubsystem::TransformWidget(const Ref<GameObject>& _object) const
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
		if (ImGui::CollapsingHeader("Transform Component"))
		{
			const Ref<TransformComponent> transform = _object->GetTransformComponent();

			ImGui::DragFloat3("Position", &transform->GetPosition()[0], DragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetPosition", ImGuiDir_Left)) transform->SetPosition(glm::vec3(0.f));
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat3("Rotation", &transform->GetRotation()[0], DragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetRotation", ImGuiDir_Left)) transform->SetRotation(glm::vec3(0.f));
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat3("Scale", &transform->GetScale()[0], DragSpeedDelta, FLT_MIN); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetScale", ImGuiDir_Left)) transform->SetScale(glm::vec3(1.f));
			ImGui::SetItemTooltip("Reset");

			ImGui::Spacing();
			ImGui::SeparatorText("Moveability");
			if (ImGui::Combo("Moveability", &transform->GetMoveabilityI(), "Static\0Dynamic\0\0"))
			{
				transform->SetMoveability(static_cast<Moveability>(transform->GetMoveabilityI()));
			}
		}
	}

	void EditorSubsystem::LightWidget(const Ref<GameObject>& _selectedObject) const
	{
		if (const Ref<Light> light = CastRef<Light>(_selectedObject))
		{
			ImGui::CollapsingHeader("Light Settings", ImGuiTreeNodeFlags_DefaultOpen);
			ImGui::ColorEdit3("Light Color", &light->GetLightColor()[0]);
			ImGui::DragFloat("Ambient Intensity", &light->GetAmbientIntensity(), DragSpeedDelta);
			ImGui::DragFloat("Diffuse Intensity", &light->GetDiffuseIntensity(), DragSpeedDelta);
			ImGui::DragFloat("Specular Intensity", &light->GetSpecularIntensity(), DragSpeedDelta);

			if (const Ref<DirectionalLight> dirLight = CastRef<DirectionalLight>(_selectedObject))
			{
				ImGui::SeparatorText("Directional Light Settings");
				ImGui::DragFloat3("Light Direction", &dirLight->GetLightDirection()[0], DragSpeedDelta);
			}

			else if (const Ref<SpotLight> spotLight = CastRef<SpotLight>(_selectedObject))
			{
				ImGui::SeparatorText("Spot Light Settings");
				ImGui::DragFloat("Edge", &spotLight->GetEdge(), DragSpeedDelta);
				ImGui::DragFloat3("Direction", &spotLight->GetDirection()[0], DragSpeedDelta);

				ImGui::SeparatorText("Attenuation");
				ImGui::DragFloat("Constant", &spotLight->GetConstant(), DragSpeedDelta);
				ImGui::DragFloat("Linear", &spotLight->GetLinear(), DragSpeedDelta);
				ImGui::DragFloat("Exponent", &spotLight->GetExponent(), DragSpeedDelta);
			}
			else if (const Ref<PointLight> pointLight = CastRef<PointLight>(_selectedObject))
			{
				ImGui::SeparatorText("Point Light Settings");
				ImGui::SeparatorText("Attenuation");
				ImGui::DragFloat("Constant", &pointLight->GetConstant(), DragSpeedDelta);
				ImGui::DragFloat("Linear", &pointLight->GetLinear(), DragSpeedDelta);
				ImGui::DragFloat("Exponent", &pointLight->GetExponent(), DragSpeedDelta);
			}
		}
	}

	void EditorSubsystem::PhysicsWidget(const Ref<GameObject>& _selectedObject) const
	{
		if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
		{
			const Ref<PhysicsComponent> comp = _selectedObject->GetPhysicsComponent();

			// Physics Simulation
			if (ImGui::Checkbox("Simulate Physics", &comp->SimulatePhysics())) comp->ToggleSimulation();

			// Simulate Gravity
			if (ImGui::Checkbox("Simulate Gravity", &comp->GetSimulateGravity())) comp->ToggleGravity();
			
			// Collision Detection
			if (ImGui::Checkbox("Collision Detection", &comp->CollisionDetectionEnabled())) comp->ToggleCollisionDetection();
			ImGui::SameLine(); const std::string state = comp->IsColliding() ? "Colliding" : "Not Colliding";
		    ImGui::Text(" State: %s", state.c_str());

			    // Mass
			ImGui::DragFloat("Mass", &comp->GetMass(), DragSpeedDelta, FLT_MIN, FLT_MAX);

			// Linear Drag
			ImGui::DragFloat("Linear Drag", &comp->GetLinearDrag(), DragSpeedDelta);

			// Angular Drag
			ImGui::DragFloat("Angular Drag", &comp->GetAngularDrag(), DragSpeedDelta);

			// Elasticity
			ImGui::DragFloat("Elasticity", &comp->GetElasticity(), DragSpeedDelta, 0.0f, 1.0f);

			if (ImGui::TreeNode("Advanced Settings"))
			{
				// Impulse Response
				ImGui::Checkbox("Impulse Resonses", &comp->GetImpulseEnabled());

				// Step Simulation Method
				static const char* stepMethods[] = { "Euler", "k2"};
				static int itemCurrent = 0; // Here we store our selection data as an index.
				const char* comboPreview = stepMethods[itemCurrent];  // Pass in the previewMatName value visible before opening the combo
				if (ImGui::BeginCombo("Step Method", comboPreview))
				{
					for (int n = 0; n < IM_ARRAYSIZE(stepMethods); n++)
					{
						const bool is_selected = (itemCurrent == n);
						if (ImGui::Selectable(stepMethods[n], is_selected))
						{
							itemCurrent = n;
							comp->SetStepMethod(static_cast<StepMethod>(n));
						}

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				ImGui::TreePop();
			}

			// Viewable Properties
			const glm::vec3 force = comp->GetForce();
			const glm::vec3& vel = comp->GetVelocity();
			const glm::vec3& avel = comp->GetAngularVelocity();
			const glm::vec3& acc = comp->GetAcceleration();
			
			ImGui::Text("Force				x: %.2f y: %.2f z: %.2f", force.x, force.y, force.z);
			ImGui::Text("Velocity			x: %.2f y: %.2f z: %.2f", vel.x, vel.y, vel.z);
			ImGui::Text("Acceleration		x: %.2f y: %.2f z: %.2f", acc.x, acc.y, acc.z);
			ImGui::Text("Angular Velocity	x: %.2f y: %.2f z: %.2f", avel.x, avel.y, avel.z);
		}
	}

	void EditorSubsystem::CollisionWidget(const Ref<GameObject>& _selectedObject) const
	{
		if (ImGui::CollapsingHeader("Collision", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (const Ref<PhysicsComponent> pComp = _selectedObject->GetPhysicsComponent())
			{
				// Collider Type
				static const char* colliderTypes[] = { "None", "Cube", "Sphere" };
				int itemCurrent = pComp->GetCollider() ? ((int)pComp->GetCollider()->GetColliderType() + 1) : 0;
				const char* previewItem = colliderTypes[itemCurrent];
				if (ImGui::BeginCombo("Collider Type", previewItem))
				{
					for (int n = 0; n < IM_ARRAYSIZE(colliderTypes); n++)
					{
						const bool is_selected = (itemCurrent == n);
						if (ImGui::Selectable(colliderTypes[n], is_selected))
						{
							itemCurrent = n;
							switch (itemCurrent)
							{
							case 0:
							{
								pComp->m_Collider = nullptr;
								DE_LOG(LogEditor, Warn, "Set collider type to none on {}", _selectedObject->GetName())
							} break;

							case 1:
							{
								pComp->m_Collider = MakeRef<CubeCollider>();
								DE_LOG(LogEditor, Warn, "Set collider type to cube on {}", _selectedObject->GetName())
							} break;

							case 2:
							{
								pComp->m_Collider = MakeRef<SphereCollider>();
								DE_LOG(LogEditor, Warn, "Set collider type to sphere on {}", _selectedObject->GetName())
							} break;
							}
						}

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (ImGui::Checkbox("Is Trigger", &pComp->IsTrigger())) pComp->ToggleTrigger();
				ImGui::SameLine(); ImGui::Text(" State: %s", pComp->GetTriggerStateS().c_str());

				// Collider Visualiser
				ImGui::Checkbox("Show Collider", &pComp->IsColliderVisible());

				// Collider settings
				if (pComp->GetCollider())
				{
					switch (pComp->GetCollider()->GetColliderType())
					{
					case ColliderType::Cube:
					{
						if (Ref<CubeCollider> sCol = CastRef<CubeCollider>(pComp->GetCollider()))
						{
							// Collider Offset
							ImGui::DragFloat3("Offset", &sCol->GetOffset()[0], DragSpeedDelta, FLT_MIN, FLT_MAX);

							// Collider Dimensions
							if (!pComp->CollisionDimensionOverride()) ImGui::BeginDisabled();
							ImGui::DragFloat3("Dimensions", &sCol->GetDimensions()[0], DragSpeedDelta, -FLT_MIN, FLT_MAX);
							if (!pComp->CollisionDimensionOverride()) ImGui::EndDisabled();
							ImGui::SameLine(); ImGui::Checkbox("## Dimesnion Override", &pComp->CollisionDimensionOverride());

							// Min Max Debug Info
							const glm::vec3& min = sCol->GetMin();
							const glm::vec3& max = sCol->GetMax();
							ImGui::Text("AABB Min x: %.2f Max x: %.2f", min.x, max.x);
							ImGui::Text("AABB Min y: %.2f Max y: %.2f", min.y, max.y);
							ImGui::Text("AABB Min z: %.2f Max z: %.2f", min.z, max.z);
						}
					} break;

					case ColliderType::Sphere:
					{
						if (Ref<SphereCollider> sCol = CastRef<SphereCollider>(pComp->GetCollider()))
						{
							ImGui::DragFloat("Radius", &sCol->GetRadius(), DragSpeedDelta, FLT_MIN, FLT_MAX);
						}
					} break;
					}
				}
			}
		}
	}

	void EditorSubsystem::RenderWidget(const Ref<GameObject>& _selectedObject)
	{
		if (ImGui::CollapsingHeader("Rendering", ImGuiTreeNodeFlags_DefaultOpen))
		{
			const Ref<RenderComponent> render = _selectedObject->GetRenderComponent();

			ImGui::Checkbox("Visible", &render->IsVisible());
			ImGui::Checkbox("Affects Lighting", &render->AffectsLighting());
			MaterialWidget(_selectedObject);
		}
	}

	void EditorSubsystem::MaterialWidget(const Ref<GameObject>& _selectedObject)
	{
		Ref<RenderComponent> rendComp = _selectedObject->GetRenderComponent();

		ImGui::SeparatorText("Material");

		// Material Settings
		if (Ref<Material>& mat = rendComp->GetMaterial())
		{
			MaterialSelectionWidget(rendComp);
	
			// Material Properties
			ImGui::DragFloat("AO", &mat->GetAO(), DragSpeedDelta, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic", &mat->GetMetallic(), DragSpeedDelta, 0.0f, 1.0f);
			ImGui::DragFloat("Roughness", &mat->GetRoughness(), DragSpeedDelta, 0.0f, 1.0f);
			// Color or Texture selectable
			{
				ImGui::Text( "Base Color");
				ImGui::ColorEdit3("Base Color", &mat->GetBaseColor()[0]);
				TextureSelectionWidget(mat);
				if(ImGui::Button("Clear Texture"))
				{
					mat->ClearBaseTexture();
				}
			}

			ImGui::DragFloat("Specular Intensity", &mat->GetSpecularIntensity());
			ImGui::DragFloat("Specular Power", &mat->GetSpecularPower());

			ImGui::Separator();
			ImGui::Text(mat->GetShader()->GetName().c_str());
			ImGui::SameLine();
			if (ImGui::Button("Edit Shader"))
			{
				m_ShaderEditor = MakeRef<ShaderEditor>(mat->GetShader());
				ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Appearing);
			}
			if(m_ShaderEditor) m_ShaderEditor->Update();
			
			//	// Texture Info
			//	ImGui::SeparatorText("Texture Info");
			//	ImGui::Text("Texture ID: %d", texture->GetTextureID());
			//	ImGui::TextWrapped("File Path : % s", texture->GetFileLocation().c_str());
			//	ImGui::Text("Size = %d x %d", texture->GetWidth(), texture->GetHeight());
			//	ImGui::Separator();

			//	// Texture Settings
			//	TextureSettings& texSettings = render->GetTextureSettings();
			//	if (ImGui::Combo("Wrap Mode", &texSettings.WrapValue, "GL_REPEAT\0GL_MIRRORED_REPEAT\0GL_CLAMP_TO_EDGE\0GL_CLAMP_TO_BORDER\0\0"))
			//	{
			//		if (texSettings.WrapValue == 0) texSettings.WrapMode = GL_REPEAT;
			//		else if (texSettings.WrapValue == 1) texSettings.WrapMode = GL_MIRRORED_REPEAT;
			//		else if (texSettings.WrapValue == 2) texSettings.WrapMode = GL_CLAMP_TO_EDGE;
			//		else if (texSettings.WrapValue == 3) texSettings.WrapMode = GL_CLAMP_TO_BORDER;
			//	}

			//	// Texture Filter
			//	if (ImGui::Combo("Filter", &texSettings.FilterValue, "GL_NEAREST\0GL_LINEAR\0GL_NEAREST_MIPMAP_NEAREST\0GL_LINEAR_MIPMAP_NEAREST\0GL_NEAREST_MIPMAP_LINEAR\0GL_LINEAR_MIPMAP_LINEAR\0\0"))
			//	{
			//		if (texSettings.FilterValue == 0) texSettings.FilterMode = GL_NEAREST;
			//		else if (texSettings.FilterValue == 1) texSettings.FilterMode = GL_LINEAR;
			//		else if (texSettings.FilterValue == 2) texSettings.FilterMode = GL_NEAREST_MIPMAP_NEAREST;
			//		else if (texSettings.FilterValue == 3) texSettings.FilterMode = GL_LINEAR_MIPMAP_NEAREST;
			//		else if (texSettings.FilterValue == 4) texSettings.FilterMode = GL_NEAREST_MIPMAP_LINEAR;
			//		else if (texSettings.FilterValue == 5) texSettings.FilterMode = GL_LINEAR_MIPMAP_LINEAR;
			//	}

			//}
		}
	}

	void EditorSubsystem::MaterialSelectionWidget(Ref<RenderComponent>& _rendComp)
	{
		if( ImGui::Button("Print Material"))
		{
		DE_LOG(Log, Info, "Material: {}", _rendComp->GetMaterial()->GetAsset()->GetAssetName());
		}
		/*if (ImGui::BeginCombo("##MaterialName", _rendComp->GetMaterial()->GetFriendlyName().c_str()))
			{
				for (auto& [fst, snd] : ResourceSubsystem::GetMaterialStore())
				{
					ImGui::PushID(fst.c_str());
					if (ImGui::Selectable(fst.c_str()))
					{
						_rendComp->SetMaterial(snd);
					}
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}*/

		if (ImGui::BeginCombo("##MaterialName", _rendComp->GetMaterial()->GetName().c_str(), ImGuiComboFlags_WidthFitPreview))
		{
			for (auto& [fst, snd] : ResourceSubsystem::GetMaterialStore())
			{
				ImGui::PushID(fst.c_str());
				if (ImGui::Selectable(fst.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap, ImVec2(250, 100)))
				{
					_rendComp->SetMaterial(snd);
				}
				ImGui::PopID();
			}
			ImGui::EndCombo();
		}
	}

	void EditorSubsystem::TextureSelectionWidget(const Ref<Material>& _material)
	{
		Ref<Texture>& texture = _material->GetBaseTexture();
		std::string preview = "None";

		// Texture Preview
		if (texture)
		{
			preview = texture->GetTextureName();
			if (unsigned int id = texture->GetTextureID())
			{
				ImGui::Image((void*)(intptr_t)id, ImVec2(100, 100)); ImGui::SameLine();
			}
		}
		
		// Texture Selection
		if (ImGui::BeginCombo("##TextureSelection", preview.c_str(), ImGuiComboFlags_WidthFitPreview))
		{
			for (auto& [fst, snd] : ResourceSubsystem::GetTextureStore())
			{
				ImGui::PushID(snd->GetTextureName().c_str());
				ImGui::Image((void*)(intptr_t)snd->GetTextureID(), ImVec2(100, 100)); ImGui::SameLine();
				if (ImGui::Selectable(snd->GetTextureName().c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap, ImVec2(250, 100)))
				{
					_material->SetBaseTexture(snd);
				}
				ImGui::PopID();
			}
			ImGui::EndCombo();
		}
	}

	void EditorSubsystem::ShaderSelectionWidget(Ref<Material>& _material)
	{
		if (!_material) return;

		ImGui::SeparatorText("Shader");
		if (Ref<Shader> shader = _material->GetShader())
		{
			if (ImGui::BeginCombo("##ShaderName", shader->GetName().c_str()))
			{
				for (auto& [fst, snd] : ResourceSubsystem::GetShaderStore())
				{
					ImGui::PushID(fst.c_str());
					if (ImGui::Selectable(fst.c_str()))
					{
						_material->SetShader(snd);
					}
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}
		}
	}

	void EditorSubsystem::MeshWidget(const Ref<GameObject>& _selectedObject)
	{
		if (ImGui::CollapsingHeader("Mesh Component"))
		{
			ImGui::SeparatorText("Mesh Settings");

			const Ref<MeshComponent> meshComp = _selectedObject->GetMeshComponent();
			std::string preview = "Empty";
			if (const Ref<Model> model = meshComp->GetModel()) preview = model->GetName();

			if (ImGui::BeginCombo("##ModelName", preview.c_str()))
			{
				for (auto& [fst, snd] : ResourceSubsystem::GetModelStore())
				{
					ImGui::PushID(fst.c_str());
					if (ImGui::Selectable(fst.c_str()))
					{
						meshComp->SetModel(snd);
						DE_LOG(LogEditor, Info, "Model on {} set to: {}", _selectedObject->GetName(), snd->GetName())
					}
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}

			ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetModel", ImGuiDir_Left))
			{
				DE_LOG(LogEditor, Info, "Model on {} reset", _selectedObject->GetName())
					meshComp->SetModel(nullptr);
			}
			ImGui::SetItemTooltip("Reset");
		}
	}

	void EditorSubsystem::CameraWidget(const Ref<GameObject>& _camera) const
	{
		if(!_camera) return;
		
		if (const Ref<Camera> camera = CastRef<Camera>(_camera))
		{
			ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
			if (ImGui::CollapsingHeader("Camera Component"))
			{
				ImGui::Checkbox("Perspective Projection", &camera->m_IsPerspective);

				ImGui::DragFloat("Fov", &camera->m_Fov, DragSpeedDelta, 1.f, 170.f); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetFov", ImGuiDir_Left)) camera->m_Fov = 45.f;
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Rotation Factor", &camera->m_RotationFactor, DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetRotationFactor", ImGuiDir_Left)) camera->m_RotationFactor = 0.1f;
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Pitch Rotation Rate", &camera->m_PitchRotationRate, DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetPitchRotationRate", ImGuiDir_Left)) camera->m_PitchRotationRate = 0.1f;

				ImGui::DragFloat("Yaw Rotation Rate", &camera->m_YawRotationRate, DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetYawRotationRate", ImGuiDir_Left)) camera->m_YawRotationRate = 0.1f;
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Near Plane", &camera->m_NearPlane, DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetNear Plane", ImGuiDir_Left)) camera->m_NearPlane = 0.1f;
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Far Plane", &camera->m_FarPlane, DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetFar Plane", ImGuiDir_Left)) camera->m_FarPlane = 100.f;
				ImGui::SetItemTooltip("Reset");

				if (ImGui::TreeNode("Advance Camera Settings"))
				{
					ImGui::DragFloat3("Forward", &camera->GetCameraFront()[0], DragSpeedDelta);
					ImGui::DragFloat3("Right", &camera->m_CameraRight[0], DragSpeedDelta);
					ImGui::DragFloat3("Up", &camera->m_CameraUp[0], DragSpeedDelta);
					
					ImGui::DragFloat3("Camera Position",
					                  &m_ActiveScene->m_ViewportCamera->GetTransformComponent()->GetPosition()[0],
					                  DragSpeedDelta);
					ImGui::SameLine();
					if (ImGui::ArrowButton("##ResetPosition", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->
						GetTransformComponent()->SetPosition(glm::vec3(0.f));
					ImGui::SetItemTooltip("Reset");

					ImGui::DragFloat3("Viewport Rotation",
					                  &m_ActiveScene->m_ViewportCamera->GetTransformComponent()->GetRotation()[0],
					                  DragSpeedDelta);
					ImGui::SameLine();
					if (ImGui::ArrowButton("##ResetRotation", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->
						GetTransformComponent()->SetRotation(glm::vec3(0.f));
					ImGui::SetItemTooltip("Reset");

					ImGui::DragFloat("m_MoveSpeed", &m_ActiveScene->m_ViewportCamera->m_MoveSpeed, DragSpeedDelta, 1.f,
					                 10.f);
					ImGui::SameLine();
					if (ImGui::ArrowButton("##ResetMoveSpeed", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->
						m_MoveSpeed;
					ImGui::SetItemTooltip("Reset");

					ImGui::DragFloat("Scoll Wheel Speed", &m_ActiveScene->m_ViewportCamera->m_MouseScrollSpeed,
					                 DragSpeedDelta, 0.1f, 10.f);

					ImGui::TreePop();
				}
			}
		}
	}
}
