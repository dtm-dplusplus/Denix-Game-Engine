#include "DePch.h"
#include "SceneSubsystem.h"
#include "WindowSubsystem.h"

SceneSubSystem* SceneSubSystem::s_SceneSubSystem{ nullptr };


void SceneSubSystem::Update(float _deltaTime)
{
	// Update the Scene Panel
	const WindowSubSystem* windowSystem = WindowSubSystem::Get();
	{
		if (!m_ActiveScene) return;

		static bool scenePanelOpen = true;
		static bool showDemoWindow = false;

		static float dragSpeed = 1.0f;
		const float dragSpeedDelta = dragSpeed * _deltaTime;

		glm::vec2 winSize;

		if (windowSystem)
		{
			winSize = windowSystem->GetWindow()->GetWindowSize();
		}

		ImGui::SetNextWindowSize(ImVec2(winSize.x / 3, winSize.y), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

		// Scene Panel 
		ImGui::Begin("Scene Panel", &scenePanelOpen, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
		{
			// Play/Pause/Stop/Reload Widgets
			{
				SceneSubSystem* sceneSystem = SceneSubSystem::Get();
				if (!m_ActiveScene->m_IsLive)
				{
					// Play Button
					if (ImGui::Button("Play"))
					{
						DE_LOG(LogSceneSubSystem, Trace, "Play Button Pressed")
						if (sceneSystem)
						{
							sceneSystem->PlayScene();

							// Skip this frame
							ImGui::End();
							return;
						}
					}
				}
				else
				{
					// Pause Button
					if (ImGui::Button("Pause"))
					{
						DE_LOG(LogSceneSubSystem, Trace, "Pause Button Pressed")
						if (sceneSystem)
						{
							sceneSystem->PauseScene();

							// Skip this frame
							ImGui::End();
							return;
						}
					}

					// Stop Button
					ImGui::SameLine();
					if (ImGui::Button("Stop"))
					{
						DE_LOG(LogSceneSubSystem, Trace, "Stop Button Pressed")
						if (sceneSystem)
						{
							sceneSystem->StopScene();

							// Skip this frame
							ImGui::End();
							return;
						}
					}
				}
			}

			// Scene Properties
			if (ImGui::CollapsingHeader("Scene Properties"))
			{
				ImGui::Checkbox("Show Demo Window", &showDemoWindow);
				if (showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);
				ImGui::DragFloat("UI Drag Speed", &dragSpeed, 0.1f, 0.1f, 10.0f);
			}

			// Camera Properties
			if (ImGui::CollapsingHeader("Camera Properties"))
			{
				ImGui::DragFloat("MoveSpeed", &m_ActiveScene->m_Camera->MoveSpeed, dragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetMoveSpeed", ImGuiDir_Left)) m_ActiveScene->m_Camera->MoveSpeed = 0.1f;
				ImGui::SetItemTooltip("Reset");

				ImGui::Checkbox("Perspective Projection", &m_ActiveScene->m_Camera->IsPerspective);

				ImGui::DragFloat("Fov", &m_ActiveScene->m_Camera->Fov, dragSpeedDelta, 1.f, 170.f); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetFov", ImGuiDir_Left)) m_ActiveScene->m_Camera->Fov = 45.f;
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Near Plane", &m_ActiveScene->m_Camera->NearPlane, dragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetNear Plane", ImGuiDir_Left)) m_ActiveScene->m_Camera->NearPlane = 0.1f;
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Far Plane", &m_ActiveScene->m_Camera->FarPlane, dragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetFar Plane", ImGuiDir_Left)) m_ActiveScene->m_Camera->FarPlane = 100.f;
				ImGui::SetItemTooltip("Reset");
			}

			// Left Pane
			static int selected = 0;
			{
				ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
				for (int i = 0; i < m_ActiveScene->m_SceneObjects.size(); i++)
				{
					// FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
					if (ImGui::Selectable(m_ActiveScene->m_SceneObjects[i]->GetName().c_str(), selected == i))
						selected = i;
				}
				ImGui::EndChild();
			}

			ImGui::SameLine();

			// Right Pane
			{
				ImGui::BeginGroup();
				ImGui::BeginChild("Object Properties", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
				ImGui::Text("Properties");
				ImGui::Separator();

				if(!m_ActiveScene->m_SceneObjects.empty())
				{
					// Transform Component
					if (const Ref<TransformComponent> transform = m_ActiveScene->m_SceneObjects[selected]->GetTransformComponent())
					{
						ImGui::SeparatorText("Transform");
						ImGui::DragFloat3("Position", &transform->GetPosition()[0], dragSpeedDelta); ImGui::SameLine();
						if (ImGui::ArrowButton("##ResetPosition", ImGuiDir_Left)) transform->SetPosition(glm::vec3(0.f));
						ImGui::SetItemTooltip("Reset");

						ImGui::DragFloat3("Rotation", &transform->GetRotation()[0], dragSpeedDelta); ImGui::SameLine();
						if (ImGui::ArrowButton("##ResetRotation", ImGuiDir_Left)) transform->SetRotation(glm::vec3(0.f));
						ImGui::SetItemTooltip("Reset");

						ImGui::DragFloat3("Scale", &transform->GetScale()[0], dragSpeedDelta); ImGui::SameLine();
						if (ImGui::ArrowButton("##ResetScale", ImGuiDir_Left)) transform->SetScale(glm::vec3(1.f));
						ImGui::SetItemTooltip("Reset");
					}

					// Physics Component
					if (const Ref<PhysicsComponent> physics = m_ActiveScene->m_SceneObjects[selected]->GetPhysicsComponent())
					{
						ImGui::SeparatorText("Physics");
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
						const char* comboPreview = stepMethods[itemCurrent];  // Pass in the preview value visible before opening the combo (it could be anything)
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
						ImGui::DragFloat3("Gravity", &gravity[0], dragSpeedDelta, -FLT_MAX, FLT_MAX);
						if (!isCustomGravity) ImGui::EndDisabled();
						ImGui::SameLine();
						if (ImGui::Checkbox("##", &isCustomGravity)) physics->ToggleGravity();

						// Mass
						ImGui::DragFloat("Mass", &mass, dragSpeedDelta, FLT_MIN, FLT_MAX);

						// Drag
						ImGui::DragFloat3("Drag", &drag[0], dragSpeedDelta);

						// Viewable Properties
						ImGui::Text("Velocity			x: %.3f y: %.3f z: %.3f", vel.x, vel.y, vel.z);
						ImGui::Text("Acceleration		x: %.3f y: %.3f z: %.3f", acc.x, acc.y, acc.z);
						ImGui::Text("Force				x: %.3f y: %.3f z: %.3f", force.x, force.y, force.z);
						ImGui::Text("Angular Velocity	x: %.3f y: %.3f z: %.3f", avel.x, avel.y, avel.z);
					}

					// Render Component
					if (const Ref<RenderComponent> render = m_ActiveScene->m_SceneObjects[selected]->GetRenderComponent())
					{
						ImGui::SeparatorText("Render");
						ImGui::ColorEdit4("Debug Color", &render->GetDebugColor()[0]);
					}
				}
				

				ImGui::EndChild();
				ImGui::EndGroup();
			}
		}
		ImGui::End();
	}

	// Update Camera
	if (const Ref<Camera> cam = m_ActiveScene->GetCamera())
	{
		cam->Aspect = windowSystem->GetWindow()->GetWindowSize();
		cam->Update(_deltaTime);

		// Camera Movement
		if (const Uint8* keyboard = SDL_GetKeyboardState(NULL))
		{
			if (keyboard[SDL_SCANCODE_W])
			{
				DE_LOG(LogScene, Info, "W Pressed")
					cam->GetTransformComponent()->GetPosition().z += cam->MoveSpeed * _deltaTime;
			}
			if (keyboard[SDL_SCANCODE_S])
			{
				DE_LOG(LogScene, Info, "S Pressed")
					cam->GetTransformComponent()->GetPosition().z -= cam->MoveSpeed * _deltaTime;
			}
			if (keyboard[SDL_SCANCODE_A])
			{
				DE_LOG(LogScene, Info, "A Pressed")
					cam->GetTransformComponent()->GetPosition().x += cam->MoveSpeed * _deltaTime;

			}
			if (keyboard[SDL_SCANCODE_D])
			{
				DE_LOG(LogScene, Info, "D Pressed")
					cam->GetTransformComponent()->GetPosition().x -= cam->MoveSpeed * _deltaTime;
			}
			if (keyboard[SDL_SCANCODE_E])
			{
				DE_LOG(LogScene, Info, "E Pressed")
					cam->GetTransformComponent()->GetPosition().y -= cam->MoveSpeed * _deltaTime;
			}
			if (keyboard[SDL_SCANCODE_Q])
			{
				DE_LOG(LogScene, Info, "Q Pressed")
					cam->GetTransformComponent()->GetPosition().y += cam->MoveSpeed * _deltaTime;
			}
		}
	}

	// Scene update implementation 
	m_ActiveScene->Update(_deltaTime);

	// Update the scene GameObjects
	for(const auto& gameObject : m_ActiveScene->m_SceneObjects)
	{
		const Ref<TransformComponent> transform = gameObject->GetTransformComponent();
		const Ref<PhysicsComponent> physics = gameObject->GetPhysicsComponent();

		if(transform && physics)
		{
			// Transform Component Update
			transform->Update(_deltaTime);

			// Physics Component Update
			if(m_ActiveScene->m_IsLive)
			{
				if(physics->m_IsSimulated)
				{
				// Temporarily store the position
				physics->m_TempPosition = transform->GetPosition();

				physics->Update(_deltaTime);

				// Update the position
				transform->SetPosition(physics->m_TempPosition);
				}
			}
		}
		
		// GameObject Update implementation
		gameObject->Update(_deltaTime);
	}

	
}