#pragma once

#include "Core.h"
#include "Scene/Scene.h"
#include "Scene/DefaultScene.h"
#include "SubSystem.h"
#include "WindowSubSystem.h"
#include "../../contrib/imgui/imgui.h"

/* Subsystem that manages the scenes
* A scene must always be loaded in order to render anything
*/ 
class SceneSubSystem: public SubSystem
{
public:
	SceneSubSystem()
	{
		s_SceneSubSystem = this;
	}

	~SceneSubSystem() override
	{
		s_SceneSubSystem = nullptr;
	}

	void Initialize() override
	{
		LoadScene(std::make_shared<DefaultScene>());

		DE_LOG(LogScene, Info, "Scene Subsystem Initialized")

		m_Initialized = true;
	}

	void Deinitialize() override
	{
		m_LoadedScenes.clear();
		m_ActiveScene = nullptr;

		DE_LOG(LogScene, Info, "Scene Subsystem Deinitialized")

		m_Initialized = false;
	}

	void Update(float _deltaTime) override;

	void ScenePanel(float _deltaTime)
	{
		if(!m_ActiveScene) return;

		static float dragSpeed = 1.0f;
		static bool showDemoWindow = false;

		const float dragSpeedDelta = dragSpeed * _deltaTime;

		static bool p_open = true;
		glm::vec2 winSize;

		if (const WindowSubSystem* windowSystem = WindowSubSystem::Get())
		{
			winSize = windowSystem->GetWindow()->GetWindowSize();
		}

		ImGui::SetNextWindowSize(ImVec2(winSize.x / 4, winSize.y), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

		// Scene Panel 
		ImGui::Begin("Scene Panel", &p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
		{
			// Play/Pause/Stop/Reload Widgets
			{
				SceneSubSystem* sceneSystem = SceneSubSystem::Get();
				if (!m_ActiveScene->m_IsLive)
				{
					// Play Button
					if (ImGui::Button("Play"))
					{
						DE_LOG(LogScene, Info, "Play Button Pressed")
							if (sceneSystem)
							{
								sceneSystem->PlayScene();
							}
					}
				}
				else
				{
					// Pause Button
					if (ImGui::Button("Pause"))
					{
						DE_LOG(LogScene, Info, "Pause Button Pressed")
							if (sceneSystem)
							{
								sceneSystem->PauseScene();
							}
					}

					// Stop Button
					ImGui::SameLine();
					if (ImGui::Button("Stop"))
					{
						DE_LOG(LogScene, Info, "Stop Button Pressed")
							if (sceneSystem)
							{
								sceneSystem->StopScene();
							}
					}
				}



				ImGui::SameLine();
				if (ImGui::Button("Reload"))
				{
					DE_LOG(LogScene, Info, "Reload Button Pressed")
						if (sceneSystem)
						{
							// Will reload the scene and play it if it was live
							sceneSystem->ReloadScene(m_ActiveScene->m_IsLive);
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

				ImGui::DragFloat("Fov", &m_ActiveScene->m_Camera->Fov, dragSpeedDelta); ImGui::SameLine();
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
					const glm::vec3& force = physics->GetForce();
					float& mass = physics->GetMass();
					glm::vec3& drag = physics->GetDrag();
					glm::vec3& gravity = physics->GetGravity();

					// Physics Simulation
					if (ImGui::Checkbox("Simulate Physics", &isSimulated)) physics->ToggleSimulation();

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
					ImGui::Text("Velocity x: %d y: %d z: %d", vel.x, vel.y, vel.z);
					ImGui::Text("Acceleration x: %d y: %d z: %d", acc.x, acc.y, acc.z);
					ImGui::Text("Force x: %d y: %d z: %d", force.x, force.y, force.z);
					ImGui::Text("Angular Velocity x: %d y: %d z: %d", avel.x, avel.y, avel.z);
				}

				// Render Component
				if (const Ref<RenderComponent> render = m_ActiveScene->m_SceneObjects[selected]->GetRenderComponent())
				{
					ImGui::SeparatorText("Render");
					ImGui::ColorEdit4("Debug Color", &render->GetDebugColor()[0]);
				}
				
				ImGui::EndChild();
				ImGui::EndGroup();
			}
		}
		ImGui::End();
	}

	bool LoadScene(Ref<Scene> _scene, const bool _isActiveScene = true)
	{
		// Check if the pointer is valid
		if (!_scene)
		{
			DE_LOG(LogScene, Critical, "Failed to load scene")
			return false;
		}

		// Load the scene
		if (!_scene->Load())
		{
			DE_LOG(LogScene, Critical, "Failed to load scene")
			return false;
		}

		DE_LOG(LogScene, Info, "Scene loaded: ", _scene->GetName())

		// Set the scene as active scene
		if(_isActiveScene)
		{
			m_ActiveScene = _scene;

			DE_LOG(LogScene, Info, "Scene set as active: ", _scene->GetName())
		}

		_scene->m_Camera = std::make_shared<Camera>();

		// Transfer ownership of the scene to the scene subsystem
		m_LoadedScenes.push_back(std::move(_scene));

		return true;
	}

	void PlayScene()
	{
		if (m_ActiveScene)
		{
			for (const auto& obj : m_ActiveScene->m_SceneObjects)
			{
				obj->BeginScene();
			}

			m_ActiveScene->BeginScene();
			
			m_ActiveScene->m_IsLive = true;

			DE_LOG(LogScene, Info, "Scene Started")
		}
	}

	void PauseScene()
	{
		DE_LOG(LogScene, Info, "Scene Paused")
	}

	void StopScene()
	{
		if (m_ActiveScene)
		{
			for (const auto& obj : m_ActiveScene->m_SceneObjects)
			{
				obj->EndScene();
			}

			m_ActiveScene->EndScene();
			
			m_ActiveScene->m_IsLive = false;

			DE_LOG(LogScene, Info, "Scene Stopped")

			ReloadScene();
		}
	}

	void ReloadScene(const bool _activateScene = false)
	{
		if (m_ActiveScene)
		{
			if(m_ActiveScene->IsLive()) StopScene();
			m_ActiveScene->Unload();
			m_ActiveScene->Load();

			if (_activateScene)
			{
				PlayScene();
			}
		}
	}
public:
	static SceneSubSystem* Get() { return s_SceneSubSystem; }

	Ref<Scene> GetActiveScene() const { return m_ActiveScene; }

	Ref<Camera> GetActiveCamera() const
	{
		if (m_ActiveScene)
		{
			return m_ActiveScene->GetCamera();
		}

		DE_LOG(LogScene, Error, "No active scene")
		return nullptr;
	}
private:
	static SceneSubSystem* s_SceneSubSystem;
	std::vector<Ref<Scene>> m_LoadedScenes;
	Ref<Scene> m_ActiveScene;

	friend class Engine;
};