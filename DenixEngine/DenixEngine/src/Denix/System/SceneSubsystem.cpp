#include "DePch.h"
#include "SceneSubsystem.h"
#include "WindowSubsystem.h"
#include "PhysicsSubSystem.h"
#include "Denix/Engine.h"
#include "imgui.h"

namespace Denix
{
	SceneSubSystem* SceneSubSystem::s_SceneSubSystem{ nullptr };



	void SceneSubSystem::Initialize()
	{
		s_PhysicsSubSystem = PhysicsSubSystem::Get();
		s_WindowSubSystem = WindowSubSystem::Get();

		DE_LOG(LogSceneSubSystem, Trace, "Scene Subsystem Initialized")

		m_Initialized = true;
	}

	void SceneSubSystem::Deinitialize()
	{
		m_LoadedScenes.clear();

		DE_LOG(LogSceneSubSystem, Trace, "Scene Subsystem Deinitialized")

		m_Initialized = false;
	}


	bool SceneSubSystem::LoadScene(const Ref<Scene>& _scene)
	{
		// Check if the pointer is valid
		if (!_scene)
		{
			DE_LOG(LogSceneSubSystem, Error, "Load Scene: Invalid scene reference")
			return false;
		}

		// Check it isn't already loaded
		if (m_LoadedScenes.contains(_scene->GetName()))
		{
			DE_LOG(LogSceneSubSystem, Error, "Load Scene: A scene name {} is already loaded", _scene->GetName())
			return false;
		}

		// Load the scene
		if (!_scene->Load())
		{
			DE_LOG(LogSceneSubSystem, Critical, "Failed to load scene")
			return false;
		}

		// Register Components
		//PhysicsSubSystem* physicsSystem = PhysicsSubSystem::Get();

		for (const auto& obj : _scene->m_SceneObjects)
		{
			if (obj->m_PhysicsComponent) obj->m_PhysicsComponent->RegisterComponent();
			if (obj->m_TransformComponent) obj->m_TransformComponent->RegisterComponent();
		}

		m_LoadedScenes[_scene->GetName()] = _scene;
		DE_LOG(LogSceneSubSystem, Trace, "Scene loaded: ", _scene->GetName())

		

		return true;
	}

	void SceneSubSystem::UnloadScene(const std::string& _name)
	{
		if (const Ref<Scene>scene = m_LoadedScenes[_name])
		{
			// Unregister Components
			//PhysicsSubSystem* physicsSystem = PhysicsSubSystem::Get();

			for (const auto& obj : m_ActiveScene->m_SceneObjects)
			{
				if (obj->m_PhysicsComponent) obj->m_PhysicsComponent->UnregisterComponent();
				if (obj->m_TransformComponent) obj->m_TransformComponent->UnregisterComponent();
			}

			m_TransformComponets.clear();

			// Unload the scene
			scene->Unload();
			m_LoadedScenes.erase(_name);

			DE_LOG(LogSceneSubSystem, Info, "Unloaded Scene: {}", _name)
			return;
		}

		DE_LOG(LogSceneSubSystem, Error, "Load Scene: Invalid scene name, or the scene isn't loaded")
	}

	void SceneSubSystem::OpenScene(const std::string& _name)
	{
		if (const Ref<Scene>scene = m_LoadedScenes[_name])
		{
			m_ActiveScene = scene;
			DE_LOG(LogSceneSubSystem, Info, "Activated Scene: {}", _name)
			return;
		}
	
		DE_LOG(LogSceneSubSystem, Error, "Cound't find Scene: {}", _name)
	}

	void SceneSubSystem::PlayScene()
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->BeginScene();
			
			DE_LOG(LogSceneSubSystem, Trace, "Scene Started")
		}
	}

	void SceneSubSystem::StopScene()
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->EndScene();
			UnloadScene(m_ActiveScene->GetName());
			m_ActiveScene = nullptr;
			
			// TEMP : this may cause memory leaks. Will move to a better solution later
			DE_LOG(LogSceneSubSystem, Trace, "Scene Stopped")

			Engine& engine = Engine::Get();
			engine.RestartScene();
		}
	}

	void SceneSubSystem::PauseScene()
	{
		DE_LOG(LogSceneSubSystem, Trace, "Scene Paused")
	}

	void SceneSubSystem::CleanRubbish()
	{
		// Cleanup rubbish objects here. TEMP loop, will be moved to a queue
		for (const auto& gameObject : m_ActiveScene->m_SceneObjects)
		{
			if (gameObject->IsRubbish())
			{
				// Unregister Components
				s_PhysicsSubSystem->UnregisterComponent(gameObject->GetPhysicsComponent());
				//gameObject->m_TransformComponent = MakeRef<TransformComponent>(m_ID);
				//gameObject->m_MeshComponent = MakeRef<MeshComponent>(m_ID);
				//gameObject->m_RenderComponent = MakeRef<RenderComponent>(m_ID);

				std::erase(m_ActiveScene->m_SceneObjects, gameObject);
			}
		}
	}

	void SceneSubSystem::Update(float _deltaTime)
	{
		// Instead of updating the object as a whole, we break it down into components and update them individually
		// Components should act as containers for the data, the logic should be implemented on the system side

		// Update Camera
		if (const Ref<Camera> cam = m_ActiveScene->GetCamera())
		{
			cam->Aspect = s_WindowSubSystem->GetWindow()->GetWindowSize();
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

		// Update Transform Components
		for (const auto& transform : m_TransformComponets)
		{
			if (transform)
			{
				glm::mat4& model = transform->GetModel();
				glm::vec3& scale = transform->GetScale();
				glm::vec3& position = transform->GetPosition();
				const glm::vec3& rotation = transform->GetRotation();

				// Update the Model matrix
				model = glm::translate(glm::mat4(1.0f), position);
				model = glm::rotate(model, glm::degrees(rotation.x), glm::vec3(1, 0, 0));
				model = glm::rotate(model, glm::degrees(rotation.y), glm::vec3(0, 1, 0));
				model = glm::rotate(model, glm::degrees(rotation.z), glm::vec3(0, 0, 1));
				model = glm::scale(model, scale);
			}
		}

		

		// Update Physics Components
		if (m_ActiveScene->m_IsLive)
		{
			s_PhysicsSubSystem->Update(_deltaTime);
		}

		// TEMP Update position from phsyics calculations
		if (m_ActiveScene->m_IsLive)
		{
			for (const auto& gameObject : m_ActiveScene->m_SceneObjects)
			{
				// TEMP Update transform components after physics calculations
				if(gameObject->m_PhysicsComponent->IsSimulated())
					gameObject->m_TransformComponent->SetPosition(gameObject->m_PhysicsComponent->m_TempPosition);
			}
		}
		

		// Update the GameObjects
		for (const auto& gameObject : m_ActiveScene->m_SceneObjects)
		{
			// TEMP Update transform components after physics calculations
			gameObject->m_PhysicsComponent->m_TempPosition = gameObject->m_TransformComponent->GetPosition();
			gameObject->Update(_deltaTime);
		}

		// Update the Scene Editor
		DragSpeedDelta = DragSpeed * _deltaTime;
		ScenePanel();
		DetailsPanel();
	}

	void SceneSubSystem::ScenePanel()
	{ 
		const glm::vec2 winSize = s_WindowSubSystem->GetWindow()->GetWindowSize();

		ImGui::SetNextWindowSize(ImVec2((winSize.x / 6), winSize.y), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

		if(ImGui::Begin("Scene Panel", &ScenePanelOpen, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse))
		{
			// Play/Pause/Deinitialize/Reload Widgets
			if (!m_ActiveScene->m_IsLive)
			{
				// Play Button
				if (ImGui::Button("Play"))
				{
					PlayScene();
					ImGui::End();
					return;
				}
			}
			else
			{
				// Pause Button
				if (ImGui::Button("Pause"))
				{
					PauseScene();
					ImGui::End();
					return;
				}

				// Stop Button
				ImGui::SameLine();
				if (ImGui::Button("Stop"))
				{
					StopScene();
					ImGui::End();
					return;
				}
			}

			// Scene Properties
			if (ImGui::CollapsingHeader("Scene Properties"))
			{
				ImGui::Checkbox("Show Demo Window", &ShowDemoWindow);
				if (ShowDemoWindow) ImGui::ShowDemoWindow(&ShowDemoWindow);
				ImGui::DragFloat("UI Drag Speed", &DragSpeed, 0.1f, 0.1f, 10.0f);
			}

			// Camera Properties
			if (ImGui::CollapsingHeader("Camera Properties"))
			{
				ImGui::DragFloat("MoveSpeed", &m_ActiveScene->m_Camera->MoveSpeed, DragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetMoveSpeed", ImGuiDir_Left)) m_ActiveScene->m_Camera->MoveSpeed = 0.1f;
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

			// Scene Objects
			ImGui::BeginChild("Scene Objects", ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
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
			ImGui::End();
		}
	}

	void SceneSubSystem::DetailsPanel()
	{
		const glm::vec2 winSize = s_WindowSubSystem->GetWindow()->GetWindowSize();
		
		ImGui::SetNextWindowSize(ImVec2((winSize.x / 6), winSize.y), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2((winSize.x / 6), 0), ImGuiCond_Once);

		if (ImGui::Begin("Details Panel", &ScenePanelOpen, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse))
		{
			if (!m_ActiveScene->m_SceneObjects.empty())
			{
				const Ref<GameObject> selectedObject = m_ActiveScene->m_SceneObjects[ObjectSelection];

				// Transform Component
				ImGui::SetNextItemOpen(true, ImGuiCond_Once); 
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
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
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

					ImGui::ColorEdit4("Debug Color", &render->GetDebugColor()[0]);
				}

				// Mesh Component
				if (ImGui::CollapsingHeader("Mesh Component"))
				{
					const Ref<MeshComponent> mesh = selectedObject->GetMeshComponent();

					ImGui::Text("Mesh: {}", mesh->GetName());
				}
			}
			ImGui::End();
		}	
	}
}
