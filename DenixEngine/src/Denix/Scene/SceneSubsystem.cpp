#include "SceneSubsystem.h"
#include "Denix/Video/WindowSubsystem.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Video/RendererSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Core/File/FileSubsystem.h"
#include "Denix/Core/Reflection/ReflectionSubsystem.h"
#include "Denix/Asset/Asset.h"

#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Core/Thread/JobSubsystem.h"
#include "Denix/Core/Thread/ThreadPrimitive.h"

#include  "yaml-cpp/yaml.h"

namespace Denix
{
	SceneSubsystem::SceneSubsystem(const Ref<Asset>& _startupScene)
	{
		m_StartupScene = _startupScene;
		m_BatchUpdateActors = true;
	}
	
	void SceneSubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogScene, Warn, "Initializing Scene Subsystem")

		// Ensure we always have a default scene
		
		
		// Load Startup scene if it exists
		if(m_StartupScene)
		{
			if(Ref<Scene> scene = CastRef<Scene>(ReflectionSubsystem::Create(m_StartupScene->GetAssetName())))
			{
				scene->m_Name = m_StartupScene->GetAssetName();
				scene->m_SceneAsset =m_StartupScene; // Reflection doesn't support constructor arguments yet
				OpenScene(scene);
			}
			else
			{
				DE_LOG(LogScene, Error, "Failed to create startup scene {}. No Reflection Class Found", m_StartupScene->GetAssetName())
				OpenScene(MakeRef<Scene>());
			}
		}
		else
		{
			OpenScene(MakeRef<Scene>());
		}
		

		DE_LOG(LogScene, Info, "Scene Subsystem Initialized")
	}

	void SceneSubsystem::Deinitialize()
	{
		DE_LOG(LogScene, Trace, "Scene Subsystem Deinitializing")

		CloseScene();
		m_StartupScene.reset();
		DE_LOG(LogScene, Trace, "Scene Subsystem Deinitialized")
		Subsystem::Deinitialize();
	}

	Ref<Camera> SceneSubsystem::GetActiveCamera()
	{
		if (s_Instance->m_ActiveScene)
		{
			return s_Instance->m_ActiveScene->GetViewportCamera();
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
		
		if(_scene->m_SceneAsset) DE_LOG(LogScene, Info, "Loaded Scene: {}", _scene->m_SceneAsset->GetAssetName())
		else DE_LOG(LogScene, Info, "Loaded Scene: {}", _scene->GetName())
		
		return true;
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

		// Close the current scene if it's open
		if (s_Instance->m_ActiveScene && s_Instance->m_ActiveScene->m_IsOpen) s_Instance->CloseScene();
		
		// Load the scene
		LoadScene(_scene);

		// Set the active scene. Take ownership of the scene pointer
		s_Instance->m_ActiveScene = std::move(_scene);
		
		// Set dependencies with new scene pointer
		RendererSubsystem::SetActiveScene(s_Instance->m_ActiveScene);
		PhysicsSubsystem::SetActiveScene(s_Instance->m_ActiveScene);
		if(EditorSubsystem::GetInstance()) EditorSubsystem::GetInstance()->SetActiveScene(s_Instance->m_ActiveScene);

		// Begin new scene
		s_Instance->m_ActiveScene->m_IsOpen = true;
		s_Instance->m_ActiveScene->BeginScene();


		// Update scene state - For shipped games, playing is the default state
		m_SceneState = SceneState::Stopped;
		DE_LOG(LogScene, Info, "Activated Scene: {}",
			s_Instance->m_ActiveScene->GetName())
	}

	void SceneSubsystem::PlayScene()
	{
		if (!s_Instance->m_ActiveScene) return;

		s_Instance->m_ActiveScene->BeginPlay();
		s_Instance->m_ActiveScene->m_IsPlaying = true;
		m_SceneState = SceneState::Playing;
		
		// Check for Game Camera
		if(const Ref<Camera> camera = s_Instance->m_ActiveScene->FindGameCamera())
		{
			// Set the camera as the active camera
			s_Instance->m_ActiveScene->m_ActiveCamera = camera;
			
		}

		DE_LOG(LogScene, Trace, "Started Playing Scene: {}", s_Instance->m_ActiveScene->GetName())
	}

	void SceneSubsystem::StopScene()
	{
		if (s_Instance->m_ActiveScene)
		{
			s_Instance->CloseScene();
			m_SceneState = SceneState::Stopped;

			// Need to establish a better way of handling scenes
			s_Instance->OpenScene(s_Instance->m_ActiveScene->m_SceneAsset);
			DE_LOG(LogScene, Trace, "Scene Stopped")
		}
	}

	void SceneSubsystem::PauseScene()
	{
		// Pause Logic
		if (m_SceneState == SceneState::Playing)
		{
			m_SceneState = SceneState::Paused;
			DE_LOG(LogScene, Trace, "Scene Paused")
		}

		// Resume logic
		else if (m_SceneState == SceneState::Paused)
		{
			m_SceneState = SceneState::Playing;
			DE_LOG(LogScene, Trace, "Scene Resumed")
		}
	}

	void SceneSubsystem::CloseScene()
	{
		if (m_ActiveScene)
		{
			if (m_ActiveScene->m_IsPlaying) m_ActiveScene->EndPlay();
			m_ActiveScene->EndScene();
			m_ActiveScene->m_IsOpen = false;
			//m_ActiveScene.reset();
		}
	}


	void SceneSubsystem::CleanRubbish()
	{
		DE_PROFILE(Clean Rubbish)

		// Cleanup rubbish actors here. 
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
		// Validate Scene
		DE_ASSERT(m_ActiveScene,"No active scene found")

		// End Profile is called after wait counter as the scene is updated in parallel
		DE_PROFILE(Scene Update)
		
		// Scene update implementation
		if(m_BatchUpdateActors)
		{
			// Submit jobs for each actor
			DE_PROFILE(AddJobBatch)
			JobSubsystem::AddJobBatch("Actor Update", Priority::NORMAL, _waitCounter, m_ActiveScene->m_Actors, &Actor::Update, _deltaTime);
		}
		else
		{
			JobSubsystem::AddJobInline("Actor Update", Priority::NORMAL, _waitCounter, [this, _deltaTime]()
			{
				for (const auto actor: m_ActiveScene->m_Actors) actor->Update(_deltaTime);
			});
		}

		// Client Scene Update
		m_ActiveScene->Update(_deltaTime);
		m_ActiveScene->DebugUI(_deltaTime);
	}

	void SceneSubsystem::SerializeScene()
	{
		SerializeScene(s_Instance->m_ActiveScene);
	}
	
	bool SceneSubsystem::SerializeScene(const Ref<Scene>& _scene)
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
			SceneEmitter << YAML::Key << "m_Actors" << YAML::BeginSeq;

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

		// For whatever reason, Clear any data held by the scene
		_scene->ClearScene();
		
		// Load the scene data from the asset file
		YAML::Node sceneNode = YAML::LoadFile(_scene->m_SceneAsset->GetAssetPath());

		// Check if the scene data is valid
		if (!sceneNode.IsDefined())
		{
			DE_LOG(LogScene, Error, "Failed to load scene asset: {}", _scene->m_SceneAsset->GetAssetPath())
			return;
		}
        	
		// Load the scene actors
		DeserializeActors(_scene, sceneNode);
	}

	bool SceneSubsystem::DeserializeActors(const Ref<Scene>& _scene,
	                                             const YAML::Node& _sceneNode)
	{
		// Load the scene actors
		YAML::Node actorsNode = _sceneNode["m_Actors"];

		if(!actorsNode.IsDefined())
		{
			DE_LOG(LogScene, Error, "Failed to load scene asset")
			return false;
		}
		
		for (const auto& actorNode : actorsNode)
		{
			// Create an actor placeholder. 
			Ref<Actor> newActor;

			// Initialize the actor base object with class type
			if(const YAML::Node& objNode = actorNode["m_Object"]["m_ClassName"]; objNode.IsDefined())
				newActor = ReflectionSubsystem::Create<Actor>(objNode.as<std::string>());

			// If reflection failed to find the class, create the actor with the default class
			if(!newActor)
			{
				newActor = MakeRef<Actor>();
				DE_LOG(LogScene, Error, "Failed to create actor with custom class. Using default class")
			}
			
			// Deserialize the actor
			newActor->Deserialize(actorNode);
			_scene->SpawnActor(newActor);
		}
		
		return true;
	}
}
