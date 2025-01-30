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
#include "Denix/Core/Thread/Counter.h"

#include  "yaml-cpp/yaml.h"

namespace Denix
{
    void SceneSubsystem::Initialize()
    {
        Subsystem::Initialize();
        DE_LOG(LogScene, Warn, "Initializing Scene Subsystem")

        // Load Startup scene if it exists
        if (Ref<Asset> startupScene = AssetSubsystem::GetStartupScene())
        {
            if (Ref<Scene> scene = CastRef<Scene>(ReflectionSubsystem::Create(startupScene->GetAssetName())))
            {
                scene->m_Name = startupScene->GetAssetName();
                scene->m_SceneAsset = startupScene;
                OpenScene(scene);
            }
            else
            {
                DE_LOG(LogScene, Error, "Failed to create startup scene {}. No Reflection Class Found",
                       m_StartupScene->GetAssetName())
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

    void SceneSubsystem::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        // Validate Scene
        DE_ASSERT(m_ActiveScene, "No active scene found")

        // End Profile is called after wait counter as the scene is updated in parallel
        DE_PROFILE(Scene Update)

        // Open Scene Request
        if (m_RequestOpenSceneAsset)
        {
            OpenScene(m_RequestOpenSceneAsset);
            m_RequestOpenSceneAsset.reset();
        }

        // Request Stop
        if (m_RequestStopScene)
        {
            m_RequestStopScene = false;
            StopScene();
        }


        // Scene update implementation
        // Expirmental Parallel Update
#ifdef DE_BUILD_DEBUG
        Ref<Counter> batchCounter = MakeRef<Counter>();
        JobSubsystem::AddJobBatch("Actor Update", Priority::NORMAL, batchCounter, m_ActiveScene->m_Actors,
                                  &Actor::Update, _deltaTime, batchCounter);
        WaitForCounter(batchCounter);
#else
		for (const auto actor: m_ActiveScene->m_Actors) actor->Update(_deltaTime, _waitCounter);
#endif

        // Client Scene Update
        m_ActiveScene->Update(_deltaTime, _waitCounter);
        if(m_SceneState != SceneState::Playing || m_ActiveScene->m_DebugToolsInPlay)
            m_ActiveScene->ToolUpdate(_deltaTime, _waitCounter);

        DE_PROFILE_END(Scene Update)
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

        DE_LOG(LogScene, Info, "Loaded Scene: {}", _scene->GetName())

        return true;
    }

    void SceneSubsystem::OpenScene(const Ref<Asset>& _sceneAsset)
    {
        if (!_sceneAsset)
        {
            DE_LOG(LogScene, Error, "Invalid Scene Asset. Loading Default Scene")
            OpenScene(MakeRef<Scene>());
            return;
        }

        if (const Ref<Scene> scene = CastRef<Scene>(ReflectionSubsystem::Create(_sceneAsset->GetAssetName())))
        {
            scene->m_SceneAsset = _sceneAsset;
            scene->m_Name = _sceneAsset->GetAssetName();
            OpenScene(scene);
        }
    }

    void SceneSubsystem::OpenScene(const Ref<Scene>& _scene)
    {
        if (!_scene)
        {
            DE_LOG(LogScene, Error, "Invalid Scene Asset. Loading Default Scene")
            OpenScene(MakeRef<Scene>());
            return;
        }

        // Close Current Scene if still playing - occurs when the scene is changed while playing
        if (s_Instance->m_ActiveScene && s_Instance->m_ActiveScene->m_IsPlaying) s_Instance->CloseScene();


        // Load the scene
        LoadScene(_scene);

        // Set the active scene. Take ownership of the scene pointer
        s_Instance->m_ActiveScene = std::move(_scene);

        // Set dependencies with new scene pointer
        PhysicsSubsystem::SetActiveScene(s_Instance->m_ActiveScene);
        if (EditorSubsystem::GetInstance()) EditorSubsystem::GetInstance()->SetActiveScene(s_Instance->m_ActiveScene);

        // Begin new scene
        s_Instance->m_ActiveScene->BeginScene();

        // Update scene state - For shipped games, playing is the default state
        if (m_SceneState != SceneState::Stopped) PlayScene();

        DE_LOG(LogScene, Info, "Activated Scene: {}", s_Instance->m_ActiveScene->GetName())
    }

    void SceneSubsystem::RequestOpenScene(const std::string& _scenePath)
    {
        Ref<Asset> reuqestAsset = MakeRef<Asset>(_scenePath);
        RequestOpenScene(reuqestAsset);
    }

    void SceneSubsystem::RequestOpenScene(const Ref<Asset>& _sceneAsset)
    {
        if (!AssetSubsystem::GetSceneAsset(_sceneAsset->GetRelativePath()))
        {
            DE_LOG(LogScene, Error, "Request Open Scene Failed")
            return;
        }

        s_Instance->m_RequestOpenSceneAsset = _sceneAsset;
    }

    void SceneSubsystem::RequestStop()
    {
        if (s_Instance->m_SceneState != SceneState::Stopped) s_Instance->m_RequestStopScene = true;
    }

    void SceneSubsystem::PlayScene()
    {
        if (!s_Instance->m_ActiveScene) return;

        s_Instance->m_ActiveScene->BeginPlay();
        s_Instance->m_ActiveScene->m_IsPlaying = true;
        m_SceneState = SceneState::Playing;

        DE_LOG(LogScene, Trace, "Started Playing Scene: {}", s_Instance->m_ActiveScene->GetName())
    }

    void SceneSubsystem::StopScene()
    {
        if (s_Instance->m_ActiveScene)
        {
            s_Instance->CloseScene();
            m_SceneState = SceneState::Stopped;
            s_Instance->m_ActiveScene->m_IsPlaying = false;

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
        if (!_scene->m_SceneAsset || _scene->m_SceneAsset->GetRelativePath().empty())
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
            SceneEmitter << YAML::Newline << YAML::Comment(_scene->GetName() + " Scene Data");
            SceneEmitter << YAML::BeginMap;
            SceneEmitter << YAML::Key << "m_Actors" << YAML::BeginSeq;

            // Serialize the actors
            for (auto& actor : _scene->m_Actors)
            {
                SceneEmitter << YAML::BeginMap;
                actor->Serialize(SceneEmitter);
                SceneEmitter << YAML::EndMap;
            }

            SceneEmitter << YAML::EndMap;

            // Write emitter data to yaml file
            FileSubsystem::WriteFile(sceneAsset->GetRelativePath(), SceneEmitter.c_str());
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

        // If for any reason data already exists in scene, Clear it
        _scene->ClearActors();

        // Load the scene data from the asset file
        YAML::Node sceneNode = YAML::LoadFile(_scene->m_SceneAsset->GetAbsolutePath());

        // Check if the scene data is valid
        if (!sceneNode.IsDefined())
        {
            DE_LOG(LogScene, Error, "Failed to load scene asset: {}", _scene->m_SceneAsset->GetAbsolutePath())
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

        if (!actorsNode.IsDefined())
        {
            DE_LOG(LogScene, Error, "Failed to load scene asset")
            return false;
        }

        for (const auto& actorNode : actorsNode)
        {
            // Create an actor placeholder. 
            Ref<Actor> newActor;

            // Initialize the actor base object with class type
            if (const YAML::Node& objNode = actorNode["m_Object"]["m_ClassName"]; objNode.IsDefined())
                newActor = ReflectionSubsystem::Create<Actor>(objNode.as<std::string>());

            // If reflection failed to find the class, create the actor with the default class
            if (!newActor)
            {
                newActor = MakeRef<Actor>();
                DE_LOG(LogScene, Error, "Failed to create actor with custom class. Using default class")
            }

            // Deserialize the actor
            newActor->Deserialize(actorNode);

            // Manually Push - We don't want to run BeginScene & BeginPlay here
            _scene->m_Actors.push_back(newActor);
            _scene->m_ActorNames.insert(newActor->GetName());
        }

        return true;
    }

    void SceneSubsystem::RenderSceneSubmission() const
    {
        if (const auto& cam = m_ActiveScene->m_ActiveCamera->GetComponent<CameraComponent>())
            RendererSubsystem::SubmitCamera({cam->GetProjectionMatrix(), cam->GetViewMatrix()});

        for (const auto& actor : m_ActiveScene->m_Actors)
        {
            if (actor->m_RenderComponent->m_IsVisible)
            {
                RendererSubsystem::RenderObject(
                    {
                        actor->m_RenderComponent->GetMaterial(),
                        actor->m_ModelComponent->GetModel(),
                        actor->GetTransformComponent()->GetModel()
                    });
            }
        }
    }
}
