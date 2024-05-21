#include "depch.h"
#include "EditorSubsystem.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Physics/Collider.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Video/Window/WindowSubsystem.h"
#include "Denix/UI/UISubsystem.h"
#include "Denix/Video/Renderer/RendererSubSystem.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Scene/Object/Shapes/Shapes.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Core/TimerSubsystem.h"

namespace Denix
{
	EditorSubsystem* EditorSubsystem::s_EditorSubsystem{ nullptr };

	void EditorSubsystem::Initialize()
	{
		s_WindowSubsystem = WindowSubsystem::Get();
		s_SceneSubsystem = SceneSubsystem::Get();
		s_InputSubsystem = InputSubsystem::Get();
		s_RendererSubsystem = RendererSubsystem::Get();
		s_UISubsystem = UISubsystem::Get();

		DE_LOG_CREATE(LogEditor)
		DE_LOG(LogEditor, Trace, "Editor Subsystem Initialized")

		m_Initialized = true;
	}

	void EditorSubsystem::Deinitialize()
	{
		DE_LOG(LogEditor, Trace, "Editor Subsystem Initialized")
		m_Initialized = false;
	}

	void EditorSubsystem::Update(float _deltaTime)
	{
		DragSpeedDelta = DragSpeed * _deltaTime;
		const glm::vec2 winSize = s_WindowSubsystem->GetWindow()->GetWindowSize();
		WinX = winSize.x;
		WinY = winSize.y;

		MenuBar();
		if (!m_ActiveScene) return;
		if(m_IsScenePanelOpen) ScenePanel();
		if (m_IsDetailsPanelOpen) DetailsPanel();
		if(m_IsTimerSettingsOpen) TimerSettings();
		if(m_IsInputPanelOpen) s_InputSubsystem->InputPanel();
		if (m_IsPhysicsSettingsOpen) PhysicsSettings();
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
		ImGui::SliderFloat("Game Speed", &TimerSubsystem::GetGameTimeSpeed(), 0.0f, 2.0f);
		ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTimeMs());
		ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());
	}

	void EditorSubsystem::MenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Quit", "Alt+F4")) 
				{
					s_WindowSubsystem->GetWindow()->RequestClose();
				}

				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::SeparatorText("Panels");
				ImGui::Checkbox("Scene Panel", &m_IsScenePanelOpen);
				ImGui::Checkbox("Details Panel", &m_IsDetailsPanelOpen);
				ImGui::Checkbox("Timer Settings", &m_IsTimerSettingsOpen);
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
					m_ObjectSelection = -1;
					s_SceneSubsystem->StopScene();
				}
			}

			// Scene Properties
			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::Checkbox("Show Demo Window", &ShowDemoWindow);
				if (ShowDemoWindow) ImGui::ShowDemoWindow(&ShowDemoWindow);
				ImGui::DragFloat("UI Drag Speed", &DragSpeed, DragSpeed, 0.1f, 10.0f);

				ImGui::ColorEdit4("Clear Color", &s_WindowSubsystem->GetWindow()->GetClearColor()[0]);
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
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
		SceneOrganizerWidget();
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
		if (ImGui::Combo("Viewport Mode", &RendererSubsystem::GetViewportMode(), "Default\0Unlit\0Wireframe\0Collision\0\0"))
		{

		}

		// Subsystems
		ImGui::Checkbox("Render Subsystem", &s_RendererSubsystem->IsEnabled());

		// Scene gravity
		ImGui::DragFloat("Scene Gravity", &s_SceneSubsystem->m_ActiveScene->GetGravity(), DragSpeedDelta, -FLT_MAX, FLT_MAX);

		// Camera Properties
		{
			ImGui::SeparatorText("Camera Properties");

			ImGui::DragFloat3("Viewport Position", &m_ActiveScene->m_ViewportCamera->GetTransformComponent()->GetPosition()[0], DragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetPosition", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->GetTransformComponent()->SetPosition(glm::vec3(0.f));
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat3("Viewport Rotation", &m_ActiveScene->m_ViewportCamera->GetTransformComponent()->GetRotation()[0], DragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetRotation", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->GetTransformComponent()->SetRotation(glm::vec3(0.f));
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat("Rotation Factor", &m_ActiveScene->m_ViewportCamera->GetRotationFactor(), DragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetRotationFactor", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->SetRotationFactor(0.01f);
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat("Pitch Rotation Rate", &m_ActiveScene->m_ViewportCamera->GetPitchRotationRate(), DragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetPitchRotationRate", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->SetPitchRotationRate(0.1f);
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat("Yaw Rotation Rate", &m_ActiveScene->m_ViewportCamera->GetYawRotationRate(), DragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetYawRotationRate", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->SetYawRotationRate(0.1f);
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat("m_MoveSpeed", &m_ActiveScene->m_ViewportCamera->GetMoveSpeed(), DragSpeedDelta, 1.f, 10.f); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetMoveSpeed", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->SetMoveSpeed(0.5f);
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat("Scoll Wheel Speed", &m_ActiveScene->m_ViewportCamera->GetMouseScrollSpeed(), DragSpeedDelta, 0.1f, 10.f);

			ImGui::Checkbox("Perspective Projection", &m_ActiveScene->m_ViewportCamera->IsPerspective());

			ImGui::DragFloat("m_Fov", &m_ActiveScene->m_ViewportCamera->GetFov(), DragSpeedDelta, 1.f, 170.f); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetFov", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->SetFov(45.f);
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat("Near Plane", &m_ActiveScene->m_ViewportCamera->GetNearPlane(), DragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetNear Plane", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->SetNearPlane(0.1f);
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat("Far Plane", &m_ActiveScene->m_ViewportCamera->GetFarPlane(), DragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetFar Plane", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->SetFarPlane(100.f);
			ImGui::SetItemTooltip("Reset");
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
							const Ref<Plane> plane = MakeRef<Plane>();
							plane->BeginScene();
							if (m_ActiveScene->IsPlaying()) plane->BeginPlay();
							m_ActiveScene->m_SceneObjects.push_back(plane);
						}
						else if (name == "Cube")
						{
							const Ref<Cube> cube = MakeRef<Cube>();
							cube->BeginScene();
							if (m_ActiveScene->IsPlaying()) cube->BeginPlay();
							m_ActiveScene->m_SceneObjects.push_back(cube);
						}
						else if (name == "Sphere")
						{
							const Ref<Sphere> sphere = MakeRef<Sphere>();
							sphere->BeginScene();
							if (m_ActiveScene->IsPlaying()) sphere->BeginPlay();
							m_ActiveScene->m_SceneObjects.push_back(sphere);
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

		if (createdObject) m_ObjectSelection = m_ActiveScene->m_SceneObjects.size() - 1;
		ImGui::EndChild();
		ImGui::Separator();
	}

	void EditorSubsystem::SceneOrganizerWidget()
	{
		// Scene Objects
		ImGui::BeginChild("SceneOrganizerWidget", ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
		for (int i = 0; i < m_ActiveScene->m_SceneObjects.size(); i++)
		{
			// FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
			if (ImGui::Selectable(m_ActiveScene->m_SceneObjects[i]->GetName().c_str(), m_ObjectSelection == i))
			{
				m_ObjectSelection = i;
			}

			if (ImGui::BeginPopupContextItem()) //uses last item id as popup id
			{
				m_ObjectSelection = i;

				// Delete Button
				if (ImGui::Button("Delete"))
				{
					m_ActiveScene->m_SceneObjects[i]->SetIsRubbish();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::EndChild();
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
				static const char* stepMethods[] = { "Euler", "k2", "k4", "Verlet" };
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

				// Minimum Velocity
				//ImGui::DragFloat("Minimum Velocity", &comp->GetMinimumVelocity(), DragSpeedDelta);
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
			ImGui::Checkbox("Base Color As Texture", &render->GetBaseColorAsTexture());

			MaterialWidget(_selectedObject);
		}
	}

	void EditorSubsystem::MaterialWidget(const Ref<GameObject>& _selectedObject)
	{
		const Ref<RenderComponent> rendComp = _selectedObject->GetRenderComponent();

		ImGui::SeparatorText("Material");

		// Material Dropdown
		Ref<Material> previewMat = rendComp->GetMaterial();
		MaterialSelectionWidget(previewMat);
		
		// Material Settings - if material is set
		if (Ref<Material> mat = rendComp->GetMaterial())
		{
			BaseMatParam& baseParam = mat->GetBaseParam();
			// Color or Texture selectable
			{
				bool texSelected = baseParam.IsTexture;
				bool colSelected = !texSelected;

				if (ImGui::Selectable("Color", colSelected, 0, ImVec2(50, 15)))
				{
					baseParam.IsTexture = false;
					ImGui::ColorEdit3("Base Color", &baseParam.Color[0]);

				}
				ImGui::SameLine();
				if (ImGui::Selectable("Texture", texSelected, 0, ImVec2(50, 15)))
				{
					baseParam.IsTexture = true;
					TextureSelectionWidget(mat);
				}
			}

			ImGui::DragFloat("Specular Intensity", &mat->GetSpecularIntensity());
			ImGui::DragFloat("Specular Power", &mat->GetSpecularPower());

		

			if (m_ShaderEditorWidget) m_ShaderEditorWidget->WidgetEditor();

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

	void EditorSubsystem::MaterialSelectionWidget(Ref<Material>& _material)
	{
		if (ImGui::BeginCombo("##MaterialName", _material->GetFriendlyName().c_str()))
		{
			for (auto& [fst, snd] : ResourceSubsystem::GetMaterialStore())
			{
				ImGui::PushID(fst.c_str());
				if (ImGui::Selectable(fst.c_str()))
				{
					_material = snd;
				}
				ImGui::PopID();
			}
			ImGui::EndCombo();
		}
	}

	void EditorSubsystem::TextureSelectionWidget(Ref<Material>& _material)
	{
		Ref<Texture> texture = _material->GetBaseParam().Texture;
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
				ImGui::PushID(fst.c_str());
				ImGui::Image((void*)(intptr_t)snd->GetTextureID(), ImVec2(100, 100)); ImGui::SameLine();
				if (ImGui::Selectable(fst.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap, ImVec2(250, 100)))
				{
					texture = snd;
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
			ImGui::SameLine(); if (ImGui::Button("Edit Shader")) m_ShaderEditorWidget = MakeRef<ShaderEditorWidget>(shader);
		}
	}

	void EditorSubsystem::MeshWidget(const Ref<GameObject>& _selectedObject)
	{
		if (ImGui::CollapsingHeader("Mesh Component"))
		{
			ImGui::SeparatorText("Mesh Settings");

			const Ref<MeshComponent> meshComp = _selectedObject->GetMeshComponent();
			std::string preview = "Empty";
			if (const Ref<Model> model = meshComp->GetModel()) preview = model->GetFriendlyName();

			if (ImGui::BeginCombo("##ModelName", preview.c_str()))
			{
				for (auto& [fst, snd] : ResourceSubsystem::GetModelStore())
				{
					ImGui::PushID(fst.c_str());
					if (ImGui::Selectable(fst.c_str()))
					{
						meshComp->SetModel(snd);
						DE_LOG(LogEditor, Info, "Model on {} set to: {}", _selectedObject->GetFriendlyName(), snd->GetName())
					}
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}

			ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetModel", ImGuiDir_Left))
			{
				DE_LOG(LogEditor, Info, "Model on {} reset", _selectedObject->GetFriendlyName())
					meshComp->SetModel(nullptr);
			}
			ImGui::SetItemTooltip("Reset");
		}
	}

	void EditorSubsystem::CameraWidget(const Ref<GameObject>& _camera) const
	{
		if (const Ref<Camera> camera = CastRef<Camera>(_camera))
		{
			ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
			if (ImGui::CollapsingHeader("Camera Component"))
			{
				ImGui::Checkbox("Perspective Projection", &camera->IsPerspective());

				ImGui::DragFloat("Fov", &camera->GetFov(), DragSpeedDelta, 1.f, 170.f); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetFov", ImGuiDir_Left)) camera->SetFov(45.f);
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Rotation Factor", &camera->GetRotationFactor(), DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetRotationFactor", ImGuiDir_Left)) camera->SetRotationFactor(0.01f);
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Pitch Rotation Rate", &camera->GetPitchRotationRate(), DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetPitchRotationRate", ImGuiDir_Left)) camera->SetPitchRotationRate(0.1f);
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Yaw Rotation Rate", &camera->GetYawRotationRate(), DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetYawRotationRate", ImGuiDir_Left)) camera->SetYawRotationRate(0.1f);
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Near Plane", &camera->GetNearPlane(), DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetNear Plane", ImGuiDir_Left)) camera->SetNearPlane(0.1f);
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Far Plane", &camera->GetFarPlane(), DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetFar Plane", ImGuiDir_Left)) camera->SetFarPlane(10000.f);
				ImGui::SetItemTooltip("Reset");
			}
		}
	}
}