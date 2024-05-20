#include "depch.h"
#include "SceneSubsystem.h"
#include "Denix/Video/Window/WindowSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Video/Renderer/RendererSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Engine.h" // TEMP

namespace Denix
{
	SceneSubsystem* SceneSubsystem::s_SceneSubsystem{ nullptr };

	void SceneSubsystem::Initialize()
	{
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
		if (m_LoadedScenes.contains(_scene->GetSceneName()))
		{
			DE_LOG(LogSceneSubSystem, Error, "Load Scene: A scene name {} is already loaded", _scene->GetSceneName())
			return false;
		}

		// Load the scene
		if (!_scene->Load())
		{
			DE_LOG(LogSceneSubSystem, Critical, "Failed to load scene")
			return false;
		}

		m_LoadedScenes[_scene->GetSceneName()] = _scene;
		DE_LOG(LogSceneSubSystem, Trace, "Scene loaded: ", _scene->GetSceneName())

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
			RendererSubsystem::SetActiveScene(m_ActiveScene);
			PhysicsSubsystem::SetActiveScene(m_ActiveScene);
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

			// Check for Game Camera
			if(const Ref<Camera> camera = m_ActiveScene->GetGameCamera())
			{
				// Set the camera as the active camera
				m_ActiveScene->m_ActiveCamera = camera;
				DE_LOG(LogSceneSubSystem, Info, "Game Camera Found: {}", camera->GetName())
			}
			else
			{
				DE_LOG(LogSceneSubSystem, Warn, "No Game Camera found. Using Viewport Camera Instead")
			}


			
			DE_LOG(LogSceneSubSystem, Trace, "Scene Playing")
		}
	}

	void SceneSubsystem::StopScene()
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->EndPlay();
			m_ActiveScene->EndScene();

			UnloadScene(m_ActiveScene->GetSceneName());
			m_ActiveScene = nullptr;
			
			DE_LOG(LogSceneSubSystem, Trace, "Scene Stopped")

			Engine::Get().RestartScene();
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
			if(!gameObject) continue;
			if (gameObject->IsRubbish())
			{
				// This will remove registered Components & other neccessary cleanups
				if (m_ActiveScene->IsPlaying()) gameObject->EndPlay();
				gameObject->EndScene();

				// Check for scene types and remove from member lists
				if (Ref<Light> light = CastRef<Light>(gameObject))
				{
					switch ((LightType)light->GetLightType())
					{ 
						case LightType::Directional:
						{
							m_ActiveScene->m_DirLight = nullptr;
						} break;
						
						case LightType::Point:
						{
							m_ActiveScene->m_PointLights.erase(std::find(m_ActiveScene->m_PointLights.begin(), m_ActiveScene->m_PointLights.end(), light));
						} break;

						case LightType::Spot:
						{
							m_ActiveScene->m_SpotLights.erase(std::find(m_ActiveScene->m_SpotLights.begin(), m_ActiveScene->m_SpotLights.end(), light));
						} break;
					}
				}
				std::erase(m_ActiveScene->m_SceneObjects, gameObject);
			}
		}
	}

	void SceneSubsystem::Update(float _deltaTime)
	{
		// Update Camera - This works regardless of the camer type (viewport/GameCamera)
		if (const Ref<Camera> cam = m_ActiveScene->m_ActiveCamera)
		{
			cam->SetAspect(WindowSubsystem::GetWindow()->GetWindowSize());
			cam->Update(_deltaTime);
		}

		// Scene update implementation 
		m_ActiveScene->Update(_deltaTime);

		
	}

	void SceneSubsystem::GameObjectsUpdate(float _deltaTime)
	{
		
	}
}