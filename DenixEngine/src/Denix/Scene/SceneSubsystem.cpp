#include "SceneSubsystem.h"
#include "Denix/Video/Window/WindowSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Video/Renderer/RendererSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Core/FileSubsystem.h"
#include "Denix/Reflection/ReflectionSubsystem.h"
#include "Denix/Resource/Asset.h"
#include <omp.h>

#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"
#include "Denix/Thread/ThreadPrimitive.h"

namespace Denix
{
	SceneSubsystem* SceneSubsystem::s_SceneSubsystem{ nullptr };

	void SceneSubsystem::Initialize()
	{
		DE_LOG(LogScene, Warn, "Initializing Scene Subsystem")

		Ref<Scene> startScene = nullptr;
		
		// Check engine config for startup scene. This is validated in the LoadConfig function
		if(m_StartupScene)
		{
			if(Ref<Scene> scene = CastRef<Scene>(ReflectionSubsystem::Create(m_StartupScene->GetAssetName())))
			{
				scene->m_SceneAsset =m_StartupScene; // Reflection doesn't support constructor arguments yet
				OpenScene(scene);
			}
			else
			{
				assert(false, "Failed to create startup scene. No Reflection Class Found");
			}
		}
		// Create a default scene
		else
		{
			DE_LOG(LogScene, Warn, "No startup scene found. Created default scene")
			OpenScene(MakeRef<Scene>());
		}

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

		// Load the scene data
		DeserializeScene(_scene);
		
		// Load the scene - This should probably be skipped now we have the deserialization
		if (!_scene->Load())
		{
			DE_LOG(LogScene, Critical, "Failed to load scene")
			return false;
		}

		s_SceneSubsystem->m_LoadedScenes[_scene->GetName()] = _scene;

		if(_scene->m_SceneAsset) DE_LOG(LogScene, Info, "Loaded Scene: {}", _scene->m_SceneAsset->GetAssetName())
		else DE_LOG(LogScene, Info, "Loaded Scene: {}", _scene->GetName())
		
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
		//if (const Ref<Scene> scene = s_SceneSubsystem->m_LoadedScenes[_name])
		if (const Ref<Scene> scene = CastRef<Scene>(ReflectionSubsystem::Create(_name)))
		{
			OpenScene(scene);
			return;
		}
	
		DE_LOG(LogScene, Error, "Cound't find Scene: {}", _name)
	}

	void SceneSubsystem::OpenScene(const Ref<Asset>& _sceneAsset)
	{
		if (!_sceneAsset)
		{
			DE_LOG(LogScene, Error, "Invalid Scene Asset")
			return;
		}

		if (const Ref<Scene> scene = CastRef<Scene>(ReflectionSubsystem::Create(_sceneAsset->GetAssetName())))
		{
			scene->m_SceneAsset = _sceneAsset;
			OpenScene(scene);
		}
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

		// Set the active scene. Take ownership of the scene pointer
		s_SceneSubsystem->m_ActiveScene = std::move(_scene);
		
		// Set dependencies with new scene pointer
		RendererSubsystem::SetActiveScene(s_SceneSubsystem->m_ActiveScene);
		PhysicsSubsystem::SetActiveScene(s_SceneSubsystem->m_ActiveScene);
		if(EditorSubsystem::Get()) EditorSubsystem::Get()->SetActiveScene(s_SceneSubsystem->m_ActiveScene);

		// Begin new scene
		s_SceneSubsystem->m_ActiveScene->BeginScene();

		DE_LOG(LogScene, Info, "Activated Scene: {}",
			s_SceneSubsystem->m_ActiveScene->GetName())
	}

	void SceneSubsystem::PlayScene()
	{
		if (!s_SceneSubsystem->m_ActiveScene) return;

		s_SceneSubsystem->m_ActiveScene->BeginPlay();

		// Check for Game Camera
		if(const Ref<Camera> camera = s_SceneSubsystem->m_ActiveScene->GetGameCamera())
		{
			// Set the camera as the active camera
			s_SceneSubsystem->m_ActiveScene->m_ActiveCamera = camera;
			DE_LOG(LogScene, Info, "Game Camera Found: {}", camera->GetName())
		}
		else
		{
			DE_LOG(LogScene, Warn, "No Game Camera found. Using Viewport Camera Instead")
		}

		DE_LOG(LogScene, Trace, "Started Playing Scene: {}", s_SceneSubsystem->m_ActiveScene->GetName())
	}

	void SceneSubsystem::StopScene()
	{
		if (s_SceneSubsystem->m_ActiveScene)
		{
			s_SceneSubsystem->m_ActiveScene->EndPlay();
			//m_ActiveScene->EndScene();

			//m_ActiveScene = nullptr;

			// Need to establish a better way of handling scenes
			
			DE_LOG(LogScene, Trace, "Scene Stopped")
		}
	}

	void SceneSubsystem::PauseScene()
	{
		DE_LOG(LogScene, Trace, "Scene Paused")
	}

	SceneSubsystem::SceneSubsystem(const Ref<Asset>& _startupScene)
	{
		s_SceneSubsystem = this;
		m_StartupScene = _startupScene;
		m_BatchUpdateActors = true;
	}

	void SceneSubsystem::CleanRubbish()
	{
		DE_PROFILE(Clean Rubbish)

		// Cleanup rubbish objects here. TEMP loop, will be moved to a queue
		for (const auto& actor : m_ActiveScene->m_Actors)
		{
			if (!actor) continue;
			if (!actor->IsRubbish()) continue;
			
			// This will remove registered Components & other neccessary cleanups
			if (m_ActiveScene->IsPlaying()) actor->EndPlay();
			actor->EndScene();

			m_ActiveScene->m_ActorNames.erase(actor->GetName());
			std::erase(m_ActiveScene->m_Actors, actor);
		}

		DE_PROFILE_END(Clean Rubbish)
	}

	void SceneSubsystem::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
	{
		DE_PROFILE(Scene Update)

		// Validate Scene
		if (!m_ActiveScene)
		{
			DE_LOG(LogScene, Error, "No active scene")
			return;
		}
		
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
		DE_PROFILE(Camera Update)
		if (const Ref<Camera> cam = m_ActiveScene->m_ActiveCamera)
		{
			cam->m_Aspect = WindowSubsystem::GetWindow()->GetWindowSize();
			cam->Update(_deltaTime);
		}
		DE_PROFILE_END(Camera Update)
		
		// Scene update implementation
		if(m_BatchUpdateActors)
		{
			// Submit jobs for each actor
			DE_PROFILE(Submit Scene Jobs)
			JobSubsystem::AddJobFor("Actor Update", Priority::NORMAL, _waitCounter, m_ActiveScene->m_Actors, &Actor::Update, _deltaTime);
			DE_PROFILE_END(Submit Scene Jobs)
		}
		else
		{
			DE_PROFILE(Actor Update)
			for (auto actor: m_ActiveScene->m_Actors)
			{
				actor->Update(_deltaTime);
			}
			DE_PROFILE_END(Actor Update)
		}

		// Client Scene Update
		m_ActiveScene->Update(_deltaTime);
		
		DE_PROFILE_END(Scene Update)
	}

	void SceneSubsystem::SerializeScene()
	{
		SerializeScene(s_SceneSubsystem->m_ActiveScene.get());
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
			DE_LOG(LogScene, Error, "Scene {} has no asset associated with it", _scene->GetName())
			return false;
		}
		
		try
		{
			Ref<Asset> sceneAsset = _scene->m_SceneAsset;

			// Create a YAML emitter to write the scene data
			YAML::Emitter SceneEmitter;

			// Serialize the scene attributes
			SceneEmitter << YAML::Comment("DE_ASSET_SCENE");
			SceneEmitter << YAML::Newline << YAML::Comment( _scene->GetName() + " Scene Data");
			SceneEmitter << YAML::BeginMap;
			SceneEmitter << YAML::Key << "m_SceneObjects" << YAML::BeginSeq;

			// Serialize the actors
			for(auto& actor : _scene->m_Actors)
			{
				SceneEmitter << YAML::BeginMap;
				actor->Serialize(SceneEmitter);
				SceneEmitter << YAML::EndMap;
			}

			SceneEmitter << YAML::EndMap;

			// Write emitter data to yaml file
			FileSubsystem::WriteFile(sceneAsset->GetAssetPath(), SceneEmitter.c_str());
			DE_LOG(LogScene, Info, "Serialized scene: {}", _scene->GetName())

			return true;
		}
		catch (const std::exception& e)
		{
			DE_LOG(LogScene, Error, "Failed to serialize scene: {}", e.what())
			return false;
		}
	}

	void SceneSubsystem::DeserializeScene(const Ref<Scene>& _scene)
	{
		// Check if the scene asset is valid
		if (!_scene->m_SceneAsset)
		{
			DE_LOG(LogScene, Error, "Scene {} has no asset associated with it", _scene->GetName())
			return;
		}
		
		// Load the scene data from the asset file
		YAML::Node sceneNode = YAML::LoadFile(_scene->m_SceneAsset->GetAssetPath());

		// Check if the scene data is valid
		if (!sceneNode)
		{
			DE_LOG(LogScene, Error, "Failed to load scene asset data: {}", _scene->m_SceneAsset->GetAssetPath())
			return;
		}
        	
		// Load the scene objects
		std::vector<Ref<Actor>> Actors;
		DeserializeSceneObjects(sceneNode, Actors);
		
		for (const auto& newActor : Actors)
			_scene->SpawnActor(newActor);
		
		DE_LOG(LogScene, Info, "Deserialized scene: {}", _scene->GetName())
	}

	bool SceneSubsystem::DeserializeSceneObjects(const YAML::Node& _sceneNode,
	                                             std::vector<Ref<Actor>>& _actors)
	{
		// Load the scene objects
		YAML::Node sceneObjectsNode = _sceneNode["m_SceneObjects"];

		if(sceneObjectsNode.IsNull())
		{
			DE_LOG(LogScene, Error, "Failed to load scene objects")
			return false;
		}
		
		for (const auto& objNode : sceneObjectsNode)
		{
			// Create an actor placeholder. 
			Ref<Actor> newActor;

			// Initialize the actor base object
			if(const YAML::Node& objDataNode = objNode["m_Object"]; objDataNode.IsDefined())
			{
				if(const YAML::Node& objClassNode = objDataNode["m_ClassName"]; objClassNode.IsDefined())
				{
					// Create the actor with the custom class. This will be used for custom actors
					newActor = ReflectionSubsystem::Create<Actor>(objClassNode.as<std::string>());
				}

				// If reflection failed to find the class, create the actor with the default class
				if(!newActor)
				{
					newActor = MakeRef<Actor>();
					DE_LOG(LogScene, Warn, "Failed to create actor with custom class. Using default class")
				}

				// Set the object GUID
				if(const YAML::Node& objGUIDNode = objDataNode["m_GUID"]; objGUIDNode.IsDefined())
				{
					newActor->m_GUID = objGUIDNode.as<unsigned int>();
				}
				else
				{
					DE_LOG(LogScene, Warn, "No object GUID found. Using default GUID")
				}
				
				// Set the object name
				if(const YAML::Node& objNameNode = objDataNode["m_Name"]; objNameNode.IsDefined())
				{
					newActor->SetName(objNameNode.as<std::string>());
				}
				else
				{
					newActor->SetName("New Actor");
					DE_LOG(LogScene, Warn, "No object name found. Using default name")
				}
			}
			
			// Deserialize the game object
			newActor->Deserialize(objNode);
			_actors.push_back(newActor);
		}
		
		return true;
	}

	void SceneSubsystem::SpawnSceneObject(const Ref<Actor>& _object)
	{
		if (s_SceneSubsystem->m_ActiveScene)
		{
			s_SceneSubsystem->m_ActiveScene->SpawnActor(_object);
		}
		else
		{
			DE_LOG(LogScene, Critical, "No active scene")
		}
	}
}
