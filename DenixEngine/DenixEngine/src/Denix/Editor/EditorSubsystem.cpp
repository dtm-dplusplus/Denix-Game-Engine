#include "depch.h"
#include "EditorSubsystem.h"

#include "imgui.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/System/SceneSubsystem.h"
#include "Denix/System/WindowSubsystem.h"
#include "Denix/System/RendererSubSystem.h"
#include "Denix/Scene/Scene.h"

namespace Denix
{
	EditorSubsystem* EditorSubsystem::s_EditorSubsystem{ nullptr };

	void EditorSubsystem::Initialize()
	{
		s_WindowSubSystem = WindowSubsystem::Get();
		s_SceneSubSystem = SceneSubsystem::Get();
		s_InputSubsystem = InputSubsystem::Get();
		s_RendererSubSystem = RendererSubsystem::Get();

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
		const glm::vec2 winSize = s_WindowSubSystem->GetWindow()->GetWindowSize();
		WinX = winSize.x;
		WinY = winSize.y;

		MenuBar();

		if (!m_ActiveScene) return;
		if(m_IsScenePanelOpen) ScenePanel();
		if (m_IsDetailsPanelOpen) DetailsPanel();
		if(m_IsInputPanelOpen) s_InputSubsystem->InputPanel();
	}

	void EditorSubsystem::MenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				/*if (ImGui::BeginMenu("Options"))
				{
					static bool enabled = true;
					ImGui::MenuItem("Enabled", "", &enabled);
					ImGui::BeginChild("child", ImVec2(0, 60), ImGuiChildFlags_Border);
					for (int i = 0; i < 10; i++)
						ImGui::Text("Scrolling Text %d", i);
					ImGui::EndChild();
					static float f = 0.5f;
					static int n = 0;
					ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
					ImGui::InputFloat("Input", &f, 0.1f);
					ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
					ImGui::EndMenu();
				}*/

				if (ImGui::MenuItem("Quit", "Alt+F4")) 
				{
					s_WindowSubSystem->GetWindow()->RequestClose();
				}

				ImGui::EndMenu();
			}
			//if (ImGui::BeginMenu("Edit"))
			//{
			//	if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			//	if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			//	ImGui::Separator();
			//	if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			//	if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			//	if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			//	ImGui::EndMenu();
			//}
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::SeparatorText("Panels");
				ImGui::Checkbox("Scene Panel", &m_IsScenePanelOpen);
				ImGui::Checkbox("Details Panel", &m_IsDetailsPanelOpen);
				ImGui::Checkbox("Input Panel", &m_IsInputPanelOpen);
		
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Toggle Fullscreen", "F11"))
				{
					s_WindowSubSystem->GetWindow()->ToggleFullscreen();
				}

				ImGui::EndMenu();
			}

			

			if (!m_ActiveScene->IsPlaying())
			{
				if (ImGui::Button("Play"))
				{
					s_SceneSubSystem->PlayScene();
				}
			}
			else
			{
				if (ImGui::Button("Pause"))
				{
					s_SceneSubSystem->PauseScene();
				}
				ImGui::SameLine();
				if (ImGui::Button("Stop"))
				{
					ObjectSelection = -1;
					s_SceneSubSystem->StopScene();
				}
			}
			

			// Scene Properties
			if (ImGui::BeginMenu("Scene Properties"))
			{
				ImGui::Checkbox("Show Demo Window", &ShowDemoWindow);
				if (ShowDemoWindow) ImGui::ShowDemoWindow(&ShowDemoWindow);
				ImGui::DragFloat("UI Drag Speed", &DragSpeed, DragSpeed, 0.1f, 10.0f);

				ImGui::ColorEdit4("Clear Color", &s_WindowSubSystem->GetWindow()->GetClearColor()[0]);
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}


	void EditorSubsystem::ScenePanel()
	{
		ImGui::SetNextWindowSize(ImVec2((WinX / 6), WinY), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(0, MenuBarHeight), ImGuiCond_Appearing); // + ViewportBarHeight
		ImGui::SetNextItemOpen(&ScenePanelOpen, ImGuiCond_Appearing);

		if (ImGui::Begin("Scene Panel", &ScenePanelOpen))
		{
			// Viewport Mode
			ImGui::Combo("Viewport Mode", &s_SceneSubSystem->GetViewportMode(), "Default\0Collider\0\0");

			// Subsystems
			ImGui::Checkbox("Render Subsystem", &s_RendererSubSystem->IsActive());

			// Scene gravity
			ImGui::DragFloat3("Scene Gravity", &s_SceneSubSystem->m_ActiveScene->GetGravity()[0], DragSpeedDelta, -FLT_MAX, FLT_MAX);

			// Camera Properties
			{
				ImGui::SeparatorText("Camera Properties");

				ImGui::DragFloat("m_MoveSpeed", &m_ActiveScene->m_ViewportCamera->GetMoveSpeed(), DragSpeedDelta, 1.f, 10.f); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetMoveSpeed", ImGuiDir_Left)) m_ActiveScene->m_ViewportCamera->SetMoveSpeed(0.5f);
				ImGui::SetItemTooltip("Reset");

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

			if (ImGui::CollapsingHeader("Registered Components"))
			{
				for (const auto& transform : s_SceneSubSystem->m_TransformComponents)
				{
					ImGui::Text("Name: %s", transform->GetName().c_str());
				}
			}


			// Scene Objects
			if (ImGui::BeginChild("Scene Objects", ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX))
			{
				bool createdObject = false;

				const static std::string shapeNames[] = { "Plane", "Cube"};
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
									m_ActiveScene->m_SceneObjects.push_back(plane);
								}
								else if (name == "Cube")
								{
									const Ref<Cube> cube = MakeRef<Cube>();
									cube->BeginScene();
									m_ActiveScene->m_SceneObjects.push_back(cube);
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

				// Set new selection to created object
				if (createdObject) ObjectSelection = m_ActiveScene->m_SceneObjects.size() - 1;

				for (int i = 0; i < m_ActiveScene->m_SceneObjects.size(); i++)
				{
					// FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
					if (ImGui::Selectable(m_ActiveScene->m_SceneObjects[i]->GetName().c_str(), ObjectSelection == i))
					{
						ObjectSelection = i;
					}

					if (ImGui::BeginPopupContextItem()) //uses last item id as popup id
					{
						ObjectSelection = i;

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
			ImGui::End();
		}
	}

	void EditorSubsystem::DetailsPanel()
	{
		const glm::vec2 winSize = s_WindowSubSystem->GetWindow()->GetWindowSize();

		ImGui::SetNextWindowSize(ImVec2((winSize.x / 6), winSize.y), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2((winSize.x / 6), MenuBarHeight), ImGuiCond_Appearing);

		if (ImGui::Begin("Details Panel", &ScenePanelOpen))
		{
			if (ObjectSelection >= 0 && ObjectSelection < m_ActiveScene->m_SceneObjects.size())
			{
				const Ref<GameObject> selectedObject = m_ActiveScene->m_SceneObjects[ObjectSelection];

				ImGui::SeparatorText(selectedObject->GetName().c_str());

				// Transform Component
				ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
				if (ImGui::CollapsingHeader("Transform Component"))
				{
					const Ref<TransformComponent> transform = selectedObject->GetTransformComponent();

					ImGui::DragFloat3("Position", &transform->GetPosition()[0], DragSpeedDelta); ImGui::SameLine();
					if (ImGui::ArrowButton("##ResetPosition", ImGuiDir_Left)) transform->SetPosition(glm::vec3(0.f));
					ImGui::SetItemTooltip("Reset");

					ImGui::DragFloat3("Rotation", &transform->GetRotation()[0], DragSpeedDelta); ImGui::SameLine();
					if (ImGui::ArrowButton("##ResetRotation", ImGuiDir_Left)) transform->SetRotation(glm::vec3(0.f));
					ImGui::SetItemTooltip("Reset");

					ImGui::DragFloat3("Scale", &transform->GetScale()[0], DragSpeedDelta); ImGui::SameLine();
					if (ImGui::ArrowButton("##ResetScale", ImGuiDir_Left)) transform->SetScale(glm::vec3(1.f));
					ImGui::SetItemTooltip("Reset");

					ImGui::Spacing();
					ImGui::SeparatorText("Moveability");
					if (ImGui::Combo("Moveability", &selectedObject->GetMoveability(), "Static\0Dynamic\0\0"))
					{
						selectedObject->SetMoveability(static_cast<Moveability>(selectedObject->GetMoveability()));
					}
				}

				// Camera Panel
				if (const Ref<Camera> camera = std::dynamic_pointer_cast<Camera>(selectedObject))
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

				// Light Panel
				if(const Ref<Light> light = std::dynamic_pointer_cast<Light>(selectedObject))
				{
					ImGui::CollapsingHeader("Light Settings", ImGuiTreeNodeFlags_DefaultOpen);
					ImGui::ColorEdit3("Light Color", &light->GetLightColor()[0]);
					ImGui::DragFloat("Ambient Intensity", &light->GetAmbientIntensity(), DragSpeedDelta);
					ImGui::DragFloat("Diffuse Intensity", &light->GetDiffuseIntensity(), DragSpeedDelta);

					if (const Ref<DirectionalLight> dirLight = std::dynamic_pointer_cast<DirectionalLight>(selectedObject))
					{
						ImGui::SeparatorText("Directional Light Settings");
						ImGui::DragFloat3("Light Direction", &dirLight->GetLightDirection()[0], DragSpeedDelta);
					}

					else if (const Ref<SpotLight> spotLight = std::dynamic_pointer_cast<SpotLight>(selectedObject))
					{
						ImGui::SeparatorText("Spot Light Settings");
						ImGui::DragFloat("Edge", &spotLight->GetEdge(), DragSpeedDelta);
						ImGui::DragFloat3("Direction", &spotLight->GetDirection()[0], DragSpeedDelta);

						ImGui::SeparatorText("Attenuation");
						ImGui::DragFloat("Constant", &spotLight->GetConstant(), DragSpeedDelta);
						ImGui::DragFloat("Linear", &spotLight->GetLinear(), DragSpeedDelta);
						ImGui::DragFloat("Exponent", &spotLight->GetExponent(), DragSpeedDelta);
					}
					else if (const Ref<PointLight> pointLight = std::dynamic_pointer_cast<PointLight>(selectedObject))
					{
						ImGui::SeparatorText("Point Light Settings");
						ImGui::SeparatorText("Attenuation");
						ImGui::DragFloat("Constant", &pointLight->GetConstant(), DragSpeedDelta);
						ImGui::DragFloat("Linear", &pointLight->GetLinear(), DragSpeedDelta);
						ImGui::DragFloat("Exponent", &pointLight->GetExponent(), DragSpeedDelta);
					}
				}

				// Physics Component
				ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
				if (ImGui::CollapsingHeader("Physics Component"))
				{
					const Ref<PhysicsComponent> physics = selectedObject->GetPhysicsComponent();

					bool isSimulated = physics->IsSimulated();
					bool isCustomGravity = physics->IsCustomGravity();
					const glm::vec3& vel = physics->GetVelocity();
					const glm::vec3& avel = physics->GetAngularVelocity();
					const glm::vec3& acc = physics->GetAcceleration();
					const glm::vec3 force = physics->GetForce();
					float& mass = physics->GetMass();
					glm::vec3& drag = physics->GetDrag();
					glm::vec3& gravity = physics->GetGravity();

					// Physics Simulation
					if (ImGui::Checkbox("Simulate Physics", &isSimulated)) physics->ToggleSimulation();

					// Step Simulation 
					const char* stepMethods[] = { "Euler", "k2", "k4", "Verlet" };
					static int itemCurrent = 0; // Here we store our selection data as an index.
					const char* comboPreview = stepMethods[itemCurrent];  // Pass in the preview value visible before opening the combo
					if (ImGui::BeginCombo("Step Method", comboPreview))
					{
						for (int n = 0; n < IM_ARRAYSIZE(stepMethods); n++)
						{
							const bool is_selected = (itemCurrent == n);
							if (ImGui::Selectable(stepMethods[n], is_selected))
							{
								itemCurrent = n;
								physics->SetStepMethod(static_cast<PhysicsComponent::StepMethod>(n));
							}

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// Gravity
					if (!isCustomGravity) ImGui::BeginDisabled();
					ImGui::DragFloat3("Gravity", &gravity[0], DragSpeedDelta, -FLT_MAX, FLT_MAX);
					if (!isCustomGravity) ImGui::EndDisabled();
					ImGui::SameLine();
					if (ImGui::Checkbox("##", &isCustomGravity)) physics->ToggleGravity();

					// Mass
					ImGui::DragFloat("Mass", &mass, DragSpeedDelta, FLT_MIN, FLT_MAX);

					// Drag
					ImGui::DragFloat3("Drag", &drag[0], DragSpeedDelta);

					// Viewable Properties
					ImGui::Text("Velocity			x: %.3f y: %.3f z: %.3f", vel.x, vel.y, vel.z);
					ImGui::Text("Acceleration		x: %.3f y: %.3f z: %.3f", acc.x, acc.y, acc.z);
					ImGui::Text("Force				x: %.3f y: %.3f z: %.3f", force.x, force.y, force.z);
					ImGui::Text("Angular Velocity	x: %.3f y: %.3f z: %.3f", avel.x, avel.y, avel.z);
				}

				// Render Component
				if (ImGui::CollapsingHeader("Render Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					const Ref<RenderComponent> render = selectedObject->GetRenderComponent();

					ImGui::Checkbox("Visible", &render->IsVisible());
					ImGui::Checkbox("Affects Lighting", &render->AffectsLighting());
					ImGui::Checkbox("Base Color As Texture", &render->GetBaseColorAsTexture());

					ImGui::SeparatorText("Material");
					if(Ref<Material> mat = render->GetMaterial())
					{
						if (ImGui::BeginCombo("##MaterialName", mat->GetFriendlyName().c_str()))
						{
							for (auto& [fst, snd] : ResourceSubsystem::GetMaterialStore())
							{
								ImGui::PushID(fst.c_str());
								if (ImGui::Selectable(fst.c_str()))
								{
									render->SetMaterial(snd);
									DE_LOG(LogEditor, Info, "Material on {} set to: {}", render->GetName(), snd->GetFriendlyName())
								}
								ImGui::PopID();
							}
							ImGui::EndCombo();
						}

						ImGui::ColorEdit3("Base Color", &mat->GetBaseColor()[0]);
						ImGui::DragFloat("Specular Intensity", &mat->GetSpecularIntensity());
						ImGui::DragFloat("Specular Power", &mat->GetSpecularPower());
					}
					
					ImGui::SeparatorText("Texture");
					if (const Ref<Texture> texture = render->GetTexture())
					{
						// Texture Preview + Selection
						ImGui::Image((void*)(intptr_t)render->GetTexture()->GetTextureID(), ImVec2(100, 100)); ImGui::SameLine();
						
						if (ImGui::BeginCombo("##TextureSelection", render->GetTexture()->GetTextureName().c_str(), ImGuiComboFlags_WidthFitPreview))
						{
							for (auto& [fst, snd] : ResourceSubsystem::GetTextureStore())
							{
								ImGui::PushID(fst.c_str());
								ImGui::Image((void*)(intptr_t)snd->GetTextureID(), ImVec2(100, 100)); ImGui::SameLine();
								if (ImGui::Selectable(fst.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap, ImVec2(250, 100)))
								{
									render->SetTexture(snd);
									DE_LOG(LogEditor, Info, "Texture on {} set to: {}", render->GetName(), snd->GetTextureName())
								}
								ImGui::PopID();
							}
							ImGui::EndCombo();
						}

						// Texture Info
						ImGui::SeparatorText("Texture Info");
						ImGui::Text("Texture ID: %d", texture->GetTextureID());
						ImGui::TextWrapped("File Path : % s", texture->GetFileLocation().c_str());
						ImGui::Text("Size = %d x %d", texture->GetWidth(), texture->GetHeight());
						ImGui::Separator();

						// Texture Settings
						TextureSettings& texSettings = render->GetTextureSettings();
						if (ImGui::Combo("Wrap Mode", &texSettings.WrapValue, "GL_REPEAT\0GL_MIRRORED_REPEAT\0GL_CLAMP_TO_EDGE\0GL_CLAMP_TO_BORDER\0\0"))
						{
							if(texSettings.WrapValue == 0) texSettings.WrapMode = GL_REPEAT;
							else if(texSettings.WrapValue == 1) texSettings.WrapMode = GL_MIRRORED_REPEAT;
							else if(texSettings.WrapValue == 2) texSettings.WrapMode = GL_CLAMP_TO_EDGE;
							else if(texSettings.WrapValue == 3) texSettings.WrapMode = GL_CLAMP_TO_BORDER;
						}
						
						// Texture Filter
						if (ImGui::Combo("Filter", &texSettings.FilterValue, "GL_NEAREST\0GL_LINEAR\0GL_NEAREST_MIPMAP_NEAREST\0GL_LINEAR_MIPMAP_NEAREST\0GL_NEAREST_MIPMAP_LINEAR\0GL_LINEAR_MIPMAP_LINEAR\0\0"))
						{
							if(texSettings.FilterValue == 0) texSettings.FilterMode = GL_NEAREST;
							else if(texSettings.FilterValue == 1) texSettings.FilterMode = GL_LINEAR;
							else if(texSettings.FilterValue == 2) texSettings.FilterMode = GL_NEAREST_MIPMAP_NEAREST;
							else if(texSettings.FilterValue == 3) texSettings.FilterMode = GL_LINEAR_MIPMAP_NEAREST;
							else if(texSettings.FilterValue == 4) texSettings.FilterMode = GL_NEAREST_MIPMAP_LINEAR;
							else if(texSettings.FilterValue == 5) texSettings.FilterMode = GL_LINEAR_MIPMAP_LINEAR;
						}	

					}
				}

				// Collider Component
				if (ImGui::CollapsingHeader("Collider Component"))
				{
					const Ref<ColliderComponent> collider = selectedObject->GetColliderComponent();
					const Ref<RenderComponent> render = collider->GetRenderComponent();

					ImGui::Checkbox("Show Collider", &render->IsVisible());
					if (ImGui::Button("Is Colliding")) collider->ToggleColliding();
				}

				// Mesh Component
				if (ImGui::CollapsingHeader("Mesh Component"))
				{
					const Ref<Denix::MeshComponent> mesh = selectedObject->GetMeshComponent();

					ImGui::Text("Mesh: {}", mesh->GetName());
				}
			}
			ImGui::End();
		}
	}

	
	void EditorSubsystem::SetActiveScene(const Ref<Scene>& _scene)
	{
		m_ActiveScene = _scene;
	}
}