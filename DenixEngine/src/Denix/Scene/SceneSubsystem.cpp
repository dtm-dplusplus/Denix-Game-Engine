#include "depch.h"
#include "SceneSubsystem.h"
#include "Denix/Video/Window/WindowSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Video/Renderer/RendererSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Engine.h" // TEMP
#include "Denix/Core/FileSubsystem.h"
#include "Denix/Resource/Asset.h"

namespace Denix
{
	SceneSubsystem* SceneSubsystem::s_SceneSubsystem{ nullptr };

	void SceneSubsystem::Initialize()
	{
		DE_LOG(LogScene, Warn, "Initializing Scene Subsystem")
		DE_LOG(LogScene, Info, "Scene Subsystem Initialized")
	}

	void SceneSubsystem::Deinitialize()
	{
		DE_LOG(LogScene, Trace, "Scene Subsystem Deinitialized")
	}


	Ref<Camera> SceneSubsystem::GetActiveCamera() const
	{
		if (m_ActiveScene)
		{
			return m_ActiveScene->GetViewportCamera();
		}

		DE_LOG(LogScene, Error, "No active scene")
		return nullptr;
	}

	bool SceneSubsystem::LoadScene(const Ref<Scene>& _scene)
	{
		// Check if the pointer is valid
		if (!_scene)
		{
			DE_LOG(LogScene, Error, "Load Scene: Invalid scene reference")
			return false;
		}

		// Check it isn't already loaded - Skip until serializer is built
		/*if (s_SceneSubsystem->m_LoadedScenes.contains(_scene->GetSceneName()))
		{
			DE_LOG(LogScene, Error, "Load Scene: A scene name {} is already loaded", _scene->GetSceneName())
			return false;
		}*/

		// Load the scene
		if (!_scene->Load())
		{
			DE_LOG(LogScene, Critical, "Failed to load scene")
			return false;
		}

		s_SceneSubsystem->m_LoadedScenes[_scene->GetSceneName()] = _scene;
		DE_LOG(LogScene, Trace, "Scene loaded: ", _scene->GetSceneName())

		return true;
	}
	
	void SceneSubsystem::UnloadScene(const std::string& _name)
	{
		if (const Ref<Scene>scene = s_SceneSubsystem->m_LoadedScenes[_name])
		{
			// Unload the scene
			scene->Unload();
			s_SceneSubsystem->m_LoadedScenes.erase(_name);

			DE_LOG(LogScene, Info, "Unloaded Scene: {}", _name)
			return;
		}

		DE_LOG(LogScene, Error, "Load Scene: Invalid scene name, or the scene isn't loaded")
	}

	void SceneSubsystem::OpenScene(const std::string& _name)
	{
		if (const Ref<Scene> scene = s_SceneSubsystem->m_LoadedScenes[_name])
		{
			OpenScene(scene);
			return;
		}
	
		DE_LOG(LogScene, Error, "Cound't find Scene: {}", _name)
	}

	void SceneSubsystem::OpenScene(const Ref<Scene>& _scene)
	{
		if(!_scene)
		{
			DE_LOG(LogScene, Error, "Invalid Scene Reference")
			return;
		}

		// Load the scene if it isn't already loaded
		if(!_scene->IsLoaded()) LoadScene(_scene);
		
		s_SceneSubsystem->m_ActiveScene = std::move(_scene);
		
		// Set dependencies with new scene pointer
		RendererSubsystem::SetActiveScene(s_SceneSubsystem->m_ActiveScene);
		PhysicsSubsystem::SetActiveScene(s_SceneSubsystem->m_ActiveScene);
		EditorSubsystem::Get()->SetActiveScene(s_SceneSubsystem->m_ActiveScene);

		// Begin new scene
		s_SceneSubsystem->m_ActiveScene->BeginScene();

		DE_LOG(LogScene, Info, "Activated Scene: {}",
			s_SceneSubsystem->m_ActiveScene->m_SceneName)
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
				DE_LOG(LogScene, Info, "Game Camera Found: {}", camera->GetName())
			}
			else
			{
				DE_LOG(LogScene, Warn, "No Game Camera found. Using Viewport Camera Instead")
			}

			DE_LOG(LogScene, Trace, "Started Playing Scene: {}", m_ActiveScene->GetSceneName())
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
			
			DE_LOG(LogScene, Trace, "Scene Stopped")

			// Temporary fix to reload scene until serializer built
			Engine::Get().PostInitialize();
		}
	}

	void SceneSubsystem::PauseScene()
	{
		DE_LOG(LogScene, Trace, "Scene Paused")
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
		if (m_ActiveScene->m_RequestStop)
		{
			StopScene();
			return;
		}

		if (m_ActiveScene->m_RequestStart)
		{
			PlayScene();
			return;
		}

		// Update Camera - This works regardless of the camer type (viewport/GameCamera)
		if (const Ref<Camera> cam = m_ActiveScene->m_ActiveCamera)
		{
			cam->m_Aspect = WindowSubsystem::GetWindow()->GetWindowSize();
			cam->Update(_deltaTime);
		}

		// Scene update implementation 
		m_ActiveScene->Update(_deltaTime);
		if(m_ActiveScene->IsPlaying()) m_ActiveScene->Update(_deltaTime);
	}

	bool SceneSubsystem::SerializeScene(const Scene* _scene)
	{
		// Check if the pointer is valid
		if (!_scene)
		{
			DE_LOG(LogScene, Error, "Failed to serialize scene: Invalid scene reference")
			return false;
		}

		// Check if there is an asset associated with the scene
		// We need the path from the asset to write the scene data
		if(!_scene->m_SceneAsset || _scene->m_SceneAsset->GetAssetPath().empty())
		{
			DE_LOG(LogScene, Error, "Scene {} has no asset associated with it", _scene->GetSceneName())
			return false;
		}
		
		try
		{
			Ref<Asset> sceneAsset = _scene->m_SceneAsset;

			// Create a YAML emitter to write the scene data
			YAML::Emitter SceneEmitter;

			// Serialize the scene attributes
			SceneEmitter << YAML::Comment("DE_ASSET_SCENE");
			SceneEmitter << YAML::Newline << YAML::Comment( _scene->m_SceneName + " Scene Data");
			SceneEmitter << YAML::BeginMap;
			SceneEmitter << YAML::Key << "m_SceneObjects" << YAML::BeginSeq;

			// Serialize the game objects
			for(auto& gameObject : _scene->m_SceneObjects)
			{
				SceneEmitter << YAML::BeginMap;
				gameObject->Serialize(SceneEmitter);
				SceneEmitter << YAML::EndMap;
			}

			SceneEmitter << YAML::EndMap;

			// Write emitter data to yaml file
			FileSubsystem::WriteFile(sceneAsset->GetAssetPath(), SceneEmitter.c_str());
			DE_LOG(LogScene, Info, "Serialized scene: {}", _scene->GetSceneName())

			return true;
		}
		catch (const std::exception& e)
		{
			DE_LOG(LogScene, Error, "Failed to serialize scene: {}", e.what())
			return false;
		}
	}

	bool SceneSubsystem::DeserializeSceneObjects(const YAML::Node& _sceneNode,
	                                             std::vector<Ref<GameObject>>& _gameObjects)
	{
		// Load the scene objects
		YAML::Node sequenceNode = _sceneNode["m_SceneObjects"];

		if(sequenceNode.IsNull())
		{
			DE_LOG(LogScene, Error, "Failed to load scene objects")
			return false;
		}
		
		for (const auto& gameObject : sequenceNode)
		{
			// Create a new game object
			Ref<GameObject> newGameObject = MakeRef<GameObject>();
				
			// Temp check for game object types
			// This will be moved to reflection system
			if(YAML::Node objData =  gameObject["m_Object"])
			{
				if(objData["m_FriendlyName"].as<std::string>().find("Light") != std::string::npos)
				{
					if (objData["m_FriendlyName"].as<std::string>().find("Dir") != std::string::npos)
					{
						newGameObject = MakeRef<DirectionalLight>();
					}

					else if (objData["m_FriendlyName"].as<std::string>().find("Point") != std::string::npos)
					{
						newGameObject = MakeRef<PointLight>();
					}

					else if (objData["m_FriendlyName"].as<std::string>().find("Spot") != std::string::npos)
					{
						newGameObject = MakeRef<SpotLight>();
					}
				}
				else if(objData["m_FriendlyName"].as<std::string>().find("Camera") != std::string::npos)
				{
					newGameObject = MakeRef<Camera>();
				}
			}
				
			// Deserialize the game object
			newGameObject->Deserialize(gameObject);
			_gameObjects.push_back(newGameObject);
		}
		
		return true;
	}

	void SceneSubsystem::SpawnSceneObject(const Ref<GameObject>& _object)
	{
		if (s_SceneSubsystem->m_ActiveScene)
		{
			s_SceneSubsystem->m_ActiveScene->SpawnSceneObject(_object);
		}
		else
		{
			DE_LOG(LogScene, Critical, "No active scene")
		}
	}
}
