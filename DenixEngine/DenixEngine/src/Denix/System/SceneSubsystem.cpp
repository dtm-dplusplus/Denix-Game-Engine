#include "depch.h"
#include "SceneSubsystem.h"
#include "WindowSubsystem.h"
#include "RendererSubsystem.h"
#include "Denix/System/PhysicsSubSystem.h"
#include "Denix/Editor/EditorSubsystem.h"

#include "Denix/Engine.h"
#include "imgui.h"
#include "Denix/Input/InputSubsystem.h"

namespace Denix
{
	SceneSubsystem* SceneSubsystem::s_SceneSubsystem{ nullptr };

	void SceneSubsystem::Initialize()
	{
		s_InputSubsystem = InputSubsystem::Get();
		s_WindowSubsystem = WindowSubsystem::Get();
		s_RendererSubsystem = RendererSubsystem::Get();
		s_PhysicsSubsystem = PhysicsSubsystem::Get();
		DE_LOG(LogSceneSubSystem, Trace, "Scene Subsystem Initialized")

		m_Initialized = true;
	}

	void SceneSubsystem::Deinitialize()
	{
		m_LoadedScenes.clear();

		DE_LOG(LogSceneSubSystem, Trace, "Scene Subsystem Deinitialized")

		m_Initialized = false;
	}


	bool SceneSubsystem::LoadScene(const Ref<Scene>& _scene)
	{
		// Check if the pointer is valid
		if (!_scene)
		{
			DE_LOG(LogSceneSubSystem, Error, "Load Scene: Invalid scene reference")
			return false;
		}

		// Check it isn't already loaded
		if (m_LoadedScenes.contains(_scene->GetFriendlyName()))
		{
			DE_LOG(LogSceneSubSystem, Error, "Load Scene: A scene name {} is already loaded", _scene->GetFriendlyName())
			return false;
		}

		// Load the scene
		if (!_scene->Load())
		{
			DE_LOG(LogSceneSubSystem, Critical, "Failed to load scene")
			return false;
		}

		m_LoadedScenes[_scene->GetFriendlyName()] = _scene;
		DE_LOG(LogSceneSubSystem, Trace, "Scene loaded: ", _scene->GetFriendlyName())

		return true;
	}

	void SceneSubsystem::UnloadScene(const std::string& _name)
	{
		if (const Ref<Scene>scene = m_LoadedScenes[_name])
		{
			// Unload the scene
			scene->Unload();
			m_LoadedScenes.erase(_name);

			DE_LOG(LogSceneSubSystem, Info, "Unloaded Scene: {}", _name)
			return;
		}

		DE_LOG(LogSceneSubSystem, Error, "Load Scene: Invalid scene name, or the scene isn't loaded")
	}

	void SceneSubsystem::OpenScene(const std::string& _name)
	{
		if (const Ref<Scene>scene = m_LoadedScenes[_name])
		{
			m_ActiveScene = scene;

			// Set dependencies with new scene pointer
			s_RendererSubsystem->SetActiveCamera(m_ActiveScene->m_Camera);
			s_PhysicsSubsystem->SetActiveScene(m_ActiveScene);
			EditorSubsystem::Get()->SetActiveScene(m_ActiveScene);

			// Begin new scene
			m_ActiveScene->BeginScene();

			DE_LOG(LogSceneSubSystem, Info, "Activated Scene: {}", _name)
			return;
		}
	
		DE_LOG(LogSceneSubSystem, Error, "Cound't find Scene: {}", _name)
	}

	void SceneSubsystem::PlayScene()
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->BeginPlay();
			s_PhysicsSubsystem->SetIsSimulating(true);
			DE_LOG(LogSceneSubSystem, Trace, "Scene Playing")
		}
	}

	void SceneSubsystem::StopScene()
	{
		if (m_ActiveScene)
		{
			// TEMP Will move to a better solution later
			s_PhysicsSubsystem->SetIsSimulating(false);

			m_ActiveScene->EndPlay();
			m_ActiveScene->EndScene();

			UnloadScene(m_ActiveScene->GetFriendlyName());
			m_ActiveScene = nullptr;
			
			DE_LOG(LogSceneSubSystem, Trace, "Scene Stopped")

			Engine& engine = Engine::Get();
			engine.RestartScene();
		}
	}

	void SceneSubsystem::PauseScene()
	{
		DE_LOG(LogSceneSubSystem, Trace, "Scene Paused")
	}

	void SceneSubsystem::CleanRubbish()
	{
		// Cleanup rubbish objects here. TEMP loop, will be moved to a queue
		for (const auto& gameObject : m_ActiveScene->m_SceneObjects)
		{
			if (gameObject->IsRubbish())
			{
				// This will removec registered Components & other neccessary cleanups
				gameObject->EndScene();
				std::erase(m_ActiveScene->m_SceneObjects, gameObject);
			}
		}
	}

	void SceneSubsystem::Update(float _deltaTime)
	{
		// Instead of updating the object as a whole, we break it down into components and update them individually
		// Components should act as containers for the data, the logic should be implemented on the system side

		// Update Camera
		if (const Ref<Camera> cam = m_ActiveScene->GetCamera())
		{
			cam->Aspect = s_WindowSubsystem->GetWindow()->GetWindowSize();
			cam->Update(_deltaTime);

			// Camera Movement
			if (const InputSubsystem* input = InputSubsystem::Get())
			{
				// XZ 
				if (input->IsKeyDown(SDL_SCANCODE_W))
				{
					cam->GetTransformComponent()->GetPosition() += cam->MoveSpeed * cam->CameraFront * _deltaTime;
				}
				if (input->IsKeyDown(SDL_SCANCODE_S))
				{
					cam->GetTransformComponent()->GetPosition() -= cam->MoveSpeed * cam->CameraFront  * _deltaTime;
				}
				if (input->IsKeyDown(SDL_SCANCODE_A))
				{
					cam->GetTransformComponent()->GetPosition() -= cam->MoveSpeed * glm::normalize(glm::cross(cam->CameraFront, cam->CameraUp)) *_deltaTime;
				}
				if (input->IsKeyDown(SDL_SCANCODE_D))
				{
					cam->GetTransformComponent()->GetPosition() += cam->MoveSpeed * glm::normalize(glm::cross(cam->CameraFront, cam->CameraUp)) * _deltaTime;
				}
			}

			// Camera Mouse Input
			const MouseData& mouse = s_InputSubsystem->GetMouseData();

			// Change move speed if mouse wheel is scrolled
			if (mouse.WheelY != 0)
			{
				cam->MoveSpeed += mouse.WheelY * 0.01 * _deltaTime;
				cam->m_IsMoveSpeedDelta = true;
			}
			else
			{
				cam->m_IsMoveSpeedDelta = false;
			}

			// Pan Camera if right mouse down
			if (mouse.Right && ((int)mouse.RelX || (int)mouse.RelY))
			{
				cam->GetTransformComponent()->GetRotation().y += mouse.RelX * cam->RotateSpeed * _deltaTime; 
				cam->GetTransformComponent()->GetRotation().x -= mouse.RelY * cam->RotateSpeed * _deltaTime; 
				cam->m_IsPanMode= true;
			}
			else
			{
				cam->m_IsPanMode = false;
			}
		}

		// Scene update implementation 
		m_ActiveScene->Update(_deltaTime);

		// Update Transform Components
		for (const auto& transform : m_TransformComponents)
		{
			if (transform)
			{
				glm::mat4& model = transform->GetModel();
				glm::vec3& scale = transform->GetScale();
				glm::vec3& position = transform->GetPosition();
				const glm::vec3& rotation = transform->GetRotation();

				// Update the Model matrix
				model = glm::translate(glm::mat4(1.0f), position);
				model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
				model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
				model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
				model = glm::scale(model, scale);
			}
		}

		// Update the GameObjects
		for (const auto& gameObject : m_ActiveScene->m_SceneObjects)
		{
			// Update the GameObject -  This will always be here
			gameObject->Update(_deltaTime);

			// Skip Camera for rendering
			if(typeid(Camera) == typeid(*gameObject)) continue;
			
			// TEMP Immediate Mode Rendering
			switch (static_cast<ViewportMode>(m_ViewportMode))
			{
				case ViewportMode::Render:
				{
					// Draw the GameObject
					if (gameObject->GetRenderComponent()->IsVisible())
					{
						s_RendererSubsystem->DrawImmediate(
							gameObject->GetRenderComponent(),
							gameObject->GetTransformComponent(),
							gameObject->GetMeshComponent());
					}

					// Draw Collider over gameobject if set to visible
					if (const Ref<ColliderComponent> collider = gameObject->GetColliderComponent())
					{
						if (Ref<RenderComponent> render = collider->GetRenderComponent())
						{
							if (render->IsVisible() && m_ViewportMode != static_cast<int>(ViewportMode::Collider))
							{
								s_RendererSubsystem->DrawImmediate(
									render,
									gameObject->GetTransformComponent(),
									collider->GetMeshComponent());
							}
						}
					}
				}
					break;

				case ViewportMode::Collider:
				{
					// TEMP Collider Rendering - This disregards whether the collider is visible or not
					if (const Ref<ColliderComponent> collider = gameObject->GetColliderComponent())
					{
						if (Ref<RenderComponent> render = collider->GetRenderComponent())
						{
							s_RendererSubsystem->DrawImmediate(
								render,
								gameObject->GetTransformComponent(),
								collider->GetMeshComponent());
						}
					}
				}
					break;

				default: break;
			}
		}
	}
}
