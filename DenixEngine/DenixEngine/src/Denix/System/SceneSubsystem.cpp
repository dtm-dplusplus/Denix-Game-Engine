#include "depch.h"
#include "SceneSubsystem.h"
#include "WindowSubsystem.h"
#include "PhysicsSubSystem.h"
#include "RendererSubSystem.h"
#include "Denix/Editor/EditorSubsystem.h"

#include "Denix/Engine.h"
#include "imgui.h"

namespace Denix
{
	SceneSubSystem* SceneSubSystem::s_SceneSubSystem{ nullptr };

	void SceneSubSystem::Initialize()
	{
		s_PhysicsSubSystem = PhysicsSubSystem::Get();
		s_WindowSubSystem = WindowSubSystem::Get();
		s_RendererSubSystem = RendererSubSystem::Get();
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

	void SceneSubSystem::UnloadScene(const std::string& _name)
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

	void SceneSubSystem::OpenScene(const std::string& _name)
	{
		if (const Ref<Scene>scene = m_LoadedScenes[_name])
		{
			m_ActiveScene = scene;
			m_ActiveScene->BeginScene();

			// Set dependencies with new scene pointer
			s_RendererSubSystem->SetActiveCamera(m_ActiveScene->m_Camera);
			EditorSubsystem::Get()->SetActiveScene(m_ActiveScene);

			DE_LOG(LogSceneSubSystem, Info, "Activated Scene: {}", _name)
			return;
		}
	
		DE_LOG(LogSceneSubSystem, Error, "Cound't find Scene: {}", _name)
	}

	void SceneSubSystem::PlayScene()
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->BeginPlay();
			
			DE_LOG(LogSceneSubSystem, Trace, "Scene Playing")
		}
	}

	void SceneSubSystem::StopScene()
	{
		if (m_ActiveScene)
		{
			// TEMP Will move to a better solution later
			m_ActiveScene->EndPlay();
			m_ActiveScene->EndScene();

			UnloadScene(m_ActiveScene->GetFriendlyName());
			m_ActiveScene = nullptr;
			
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
		if (m_ActiveScene->m_IsPlaying)
		{
			s_PhysicsSubSystem->Update(_deltaTime);
		}

		// TEMP Update position from phsyics calculations
		if (m_ActiveScene->m_IsPlaying)
		{
			for (const auto& gameObject : m_ActiveScene->m_SceneObjects)
			{
				// TEMP Update transform components after physics calculations
				if (gameObject->m_PhysicsComponent->IsSimulated())
					gameObject->m_TransformComponent->SetPosition(gameObject->m_PhysicsComponent->m_TempPosition);
			}
		}

		// Update the GameObjects
		for (const auto& gameObject : m_ActiveScene->m_SceneObjects)
		{
			// TEMP Update transform components after physics calculations
			gameObject->m_PhysicsComponent->m_TempPosition = gameObject->m_TransformComponent->GetPosition();

			// Update the GameObject -  This will always be here
			gameObject->Update(_deltaTime);

			// TEMP Immediate Mode Rendering
			s_RendererSubSystem->DrawImmediate(
				gameObject->GetRenderComponent(),
				gameObject->GetTransformComponent(),
				gameObject->GetMeshComponent());

		}

	}
}
