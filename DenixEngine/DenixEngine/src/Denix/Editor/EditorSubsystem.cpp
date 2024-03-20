#include "depch.h"
#include "EditorSubsystem.h"

#include "imgui.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/System/SceneSubsystem.h"
#include "Denix/System/WindowSubsystem.h"
#include "Denix/Scene/Scene.h"

namespace Denix
{
	EditorSubsystem* EditorSubsystem::s_EditorSubsystem{ nullptr };

	void EditorSubsystem::Initialize()
	{
		s_WindowSubSystem = WindowSubsystem::Get();
		s_SceneSubSystem = SceneSubsystem::Get();
		s_InputSubsystem = InputSubsystem::Get();

		DE_LOG(Log, Trace, "Editor Subsystem Initialized")

		m_Initialized = true;
	}

	void EditorSubsystem::Deinitialize()
	{
		DE_LOG(Log, Trace, "Editor Subsystem Initialized")
		m_Initialized = false;
	}

	void EditorSubsystem::Update(float _deltaTime)
	{
		DragSpeedDelta = DragSpeed * _deltaTime;
		const glm::vec2 winSize = s_WindowSubSystem->GetWindow()->GetWindowSize();
		WinX = winSize.x;
		WinY = winSize.y;

		MenuBar();
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
			// Scene Properties
			ImGui::Combo("Viewport Mode", &s_SceneSubSystem->GetViewportMode(), "Default\0Collider\0\0");

			// Camera Properties
			{
				ImGui::SeparatorText("Camera Properties");

				ImGui::Text("Is Panning: {}", m_ActiveScene->m_Camera->m_IsPanMode ? "true" : "false");
				ImGui::Text("Is Move Speed Delta: {}", m_ActiveScene->m_Camera->m_IsMoveSpeedDelta ? "true" : "false");

				ImGui::DragFloat("MoveSpeed", &m_ActiveScene->m_Camera->MoveSpeed, DragSpeedDelta, 1.f, 10.f); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetMoveSpeed", ImGuiDir_Left)) m_ActiveScene->m_Camera->MoveSpeed = 0.5f;
				ImGui::SetItemTooltip("Reset");

				ImGui::Checkbox("Perspective Projection", &m_ActiveScene->m_Camera->IsPerspective);

				ImGui::DragFloat("Fov", &m_ActiveScene->m_Camera->Fov, DragSpeedDelta, 1.f, 170.f); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetFov", ImGuiDir_Left)) m_ActiveScene->m_Camera->Fov = 45.f;
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Near Plane", &m_ActiveScene->m_Camera->NearPlane, DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetNear Plane", ImGuiDir_Left)) m_ActiveScene->m_Camera->NearPlane = 0.1f;
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Far Plane", &m_ActiveScene->m_Camera->FarPlane, DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetFar Plane", ImGuiDir_Left)) m_ActiveScene->m_Camera->FarPlane = 100.f;
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

				const char* names[] = { "Plane", "Cube", "Triangle" };

				if (ImGui::Button("Add"))
					ImGui::OpenPopup("add_object_popup");

				if (ImGui::BeginPopup("add_object_popup"))
				{
					for (auto& name : names)
					{
						if (ImGui::Selectable(name))
						{
							createdObject = true;
							if (strcmp(name, "Plane") == 0)
							{
								const Ref<Plane> plane = MakeRef<Plane>();
								plane->BeginScene();
								m_ActiveScene->m_SceneObjects.push_back(plane);
							}
							else if (strcmp(name, "Cube") == 0)
							{
								const Ref<Cube> cube = MakeRef<Cube>();
								cube->BeginScene();
								m_ActiveScene->m_SceneObjects.push_back(cube);
							}
							else if (strcmp(name, "Triangle") == 0)
							{
								const Ref<Triangle> tri = MakeRef<Triangle>();
								tri->BeginScene();
								m_ActiveScene->m_SceneObjects.push_back(tri);
							}
						}
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
				if (ImGui::CollapsingHeader("Render Component"))
				{
					const Ref<RenderComponent> render = selectedObject->GetRenderComponent();

					ImGui::Checkbox("Visible", &render->IsVisible());
					if (ImGui::Combo("Draw Mode", &render->GetDrawMode(), "Points\0Lines\0Line Loop\0Line Strip\0Triangles\0\0"))
					{
						DE_LOG(Log, Trace, "Draw Mode Changed to: {}", render->GetDrawMode());
					}
					ImGui::ColorEdit4("Debug Color", &render->GetDebugColor()[0]);
				}

				// Collider Component
				if (ImGui::CollapsingHeader("Collider Component"))
				{
					const Ref<ColliderComponent> collider = selectedObject->GetColliderComponent();
					const Ref<RenderComponent> render = collider->GetRenderComponent();

					ImGui::Checkbox("Show Collider", &render->IsVisible());
					if (ImGui::Button("Is Colliding")) collider->ToggleColliding();
					ImGui::ColorEdit4("Collider Color", &render->GetDebugColor()[0]);
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
