#include "Engine.h"

#include <GL/glew.h>
#include <SDL3/SDL.h>


#include "yaml-cpp/yaml.h"

#include "Denix/Scene/Component/AudioComponent.h"
#include "Denix/Core/Subsystem.h"
#include "Denix/Core/Thread/JobSubsystem.h"
#include "Denix/Core/Reflection/ReflectionSubsystem.h"
#include "Denix/Video/WindowSubsystem.h"
#include "Denix/UI/UISubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Video/RendererSubsystem.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Core/File/FileSubsystem.h"
#include "Denix/Core/Time/TimerSubsystem.h"
#include "Denix/Audio/AudioSubsystem.h"
#include "Event/EventSubsystem.h"
#include "Profile/ProfileSubsystem.h"
#include "Scene/Actor/Shapes.h"


namespace Denix
{
    void Engine::EngineLoop()
    {
        DE_LOG(LogEngine, Info, "Engine Loop Started")

        while (m_WindowSubsystem->m_Window->IsOpen())
        {
            // Setup timer system for the new frame.
            m_TimerSubsystem->BeginFrame();
            //DE_LOG(LogEngine, Trace, "Frame Start")

            // Poll input & Events. Events will be dispatched to the appropriate subsystems
            Ref<Counter> eventCounter = MakeRef<Counter>();
            m_JobSubsystem->AddJobInline("Input Poll", Priority::NORMAL, eventCounter, &EventSubsystem::Update,
                                         m_EventSubsystem, m_TimerSubsystem->m_DeltaTime);
            //DE_LOG(LogEngine, Trace, "Input Poll")

            // Clear the offscreen frame buffer
            Ref<Counter> clearCounter = MakeRef<Counter>();
            m_JobSubsystem->AddJobInline("Clear Frame Buffer", Priority::NORMAL, clearCounter,
                                         &WindowSubsystem::NewFrame, m_WindowSubsystem);
            //DE_LOG(LogEngine, Trace, "Clear Frame Buffer")

            // Update the physics system. Collision detection and resolution will be here
            Ref<Counter> physicsCounter = MakeRef<Counter>();
            m_JobSubsystem->AddJob("Physics Update", Priority::NORMAL, physicsCounter, &PhysicsSubsystem::Update,
                                   m_PhysicsSubsystem, m_TimerSubsystem->m_DeltaTime);
            WaitForCounter(physicsCounter);

            // Update the scene. The majority of the client game logic will be here. Do this in parallel with the rendering
            Ref<Counter> sceneCounter = MakeRef<Counter>();
            m_JobSubsystem->AddJob("Scene Update", Priority::NORMAL, sceneCounter, &SceneSubsystem::Update,
                                   m_SceneSubsystem, m_TimerSubsystem->m_DeltaTime, sceneCounter);
            WaitForCounter(sceneCounter);
            DE_PROFILE_END(Scene Update)

            // Update Physics Components
            DE_PROFILE(Physics Post)
            for (const auto& actor : m_SceneSubsystem->m_ActiveScene->m_Actors)
            {
                if (const Ref<PhysicsComponent> physcComp = actor->GetPhysicsComponent())
                {
                    if (!physcComp->m_PxActor) continue;

                    if (physx::PxRigidDynamic* pxActor = physcComp->m_PxActor->is<physx::PxRigidDynamic>())
                    {
                        const glm::vec3& pos = actor->GetTransformComponent()->GetPosition();
                        pxActor->setGlobalPose(physx::PxTransform(pos.x, pos.y, pos.z));
                    }
                }
            }
            DE_PROFILE_END(Physics Post)

            // Update the UI & Editor for any changes
            Ref<Counter> uiCounter = MakeRef<Counter>();
            m_JobSubsystem->AddJob("UI Update", Priority::NORMAL, uiCounter, &UISubsystem::Update, m_UISubsystem,
                                   m_TimerSubsystem->m_DeltaTime);
            WaitForCounter(uiCounter);
            //DE_LOG(LogEngine, Trace, "UI Update")

            // Run on main due to opengl context when initializing the scene
            Ref<Counter> editorCounter = MakeRef<Counter>();
            m_JobSubsystem->AddJobInline("Update Editor", Priority::NORMAL, editorCounter, &EditorSubsystem::Update,
                                         m_EditorSubsystem, m_TimerSubsystem->m_DeltaTime);
            //DE_LOG(LogEngine, Trace, "Update Editor")

            // Render the scene. This runs on the main thread as it requires the opengl context
            Ref<Counter> renderCounter = MakeRef<Counter>();
            m_JobSubsystem->AddJobInline("Render Scene", Priority::NORMAL, renderCounter,
                                         &RendererSubsystem::RenderScene, m_RendererSubsystem);
            //DE_LOG(LogEngine, Trace, "Render Scene")

            // Unbind from the viewport framebuffer & Draw the framebuffer texture to the default screen buffer
            Ref<Counter> drawCounter = MakeRef<Counter>();
            m_JobSubsystem->AddJobInline("Draw Viewport", Priority::NORMAL, drawCounter, &WindowSubsystem::PresentFrame,
                                         m_WindowSubsystem);
            //DE_LOG(LogEngine, Trace, "Draw Viewport")

            // Run the garbage collector
            //DE_LOG(LogEngine, Trace, "Clean Rubbish")
            Ref<Counter> garbageCounter = MakeRef<Counter>();
            m_JobSubsystem->AddJob("Clean Rubbish", Priority::NORMAL, garbageCounter, &SceneSubsystem::CleanRubbish,
                                   m_SceneSubsystem);
            WaitForCounter(garbageCounter);

            m_TimerSubsystem->EndFrame();
            m_FrameCount++;
            //DE_LOG(LogEngine, Trace, "Frame End")
        }
    }

    void Engine::Run()
    {
        try
        {
            PreInitialize();
            Initialize();
            EngineLoop();
        }
        catch (const std::exception& e)
        {
            DE_LOG(LogEngine, Critical, "Unhandled exception: {0}", e.what());
        }
        
        Deinitialize();
    }
    
    void Engine::PreInitialize()
    {
        // Set the engine instance
        s_Engine = shared_from_this();

        // Initialize Logger
        Logger::Initialize();

        DE_LOG_CREATE(LogEngine)
        DE_LOG_CREATE(LogCore)
        DE_LOG_CREATE(LogFile)
        DE_LOG_CREATE(LogTimer)
        DE_LOG_CREATE(LogScene)
        DE_LOG_CREATE(LogAsset)
        DE_LOG_CREATE(LogAudio)
        DE_LOG_CREATE(LogEvent)
        DE_LOG_CREATE(LogInput)
        DE_LOG_CREATE(LogPhysics)
        DE_LOG_CREATE(LogProfile)
        DE_LOG_CREATE(LogReflection)
        DE_LOG_CREATE(LogThread)
        DE_LOG_CREATE(LogJob)
        DE_LOG_CREATE(LogUI)
        DE_LOG_CREATE(LogRender)
        DE_LOG_CREATE(LogGL)
        DE_LOG_CREATE(LogWindow)
        DE_LOG_CREATE(LogShader)
        DE_LOG_CREATE(LogEditor)

        // Initialize the file subsystem and establish the project paths
        m_FileSubsystem = InitalizeSubsystem<FileSubsystem>();

        // Load the engine config from project file. This will set the startup scene and validate the engine config
        LoadConfig();

        // Register all classes that need to be reflected here. 
        m_ReflectionSubsystem = InitalizeSubsystem<ReflectionSubsystem>();

        // Register classes - This will be moved to some kind of pre build event & parser in the future
        ReflectionSubsystem::Register<BaseObject>();
        ReflectionSubsystem::Register<Scene>();
        ReflectionSubsystem::Register<Actor>();
        ReflectionSubsystem::Register<Camera>();
        ReflectionSubsystem::Register<Cube>();
        ReflectionSubsystem::Register<Sphere>();
        ReflectionSubsystem::Register<Plane>();

        ReflectionSubsystem::Register<TransformComponent>();
        ReflectionSubsystem::Register<RenderComponent>();
        ReflectionSubsystem::Register<MeshComponent>();

        ReflectionSubsystem::Register<PhysicsComponent>();
        ReflectionSubsystem::Register<Collider>();
        ReflectionSubsystem::Register<CubeCollider>();
        ReflectionSubsystem::Register<SphereCollider>();

        ReflectionSubsystem::Register<AudioComponent>();
        ReflectionSubsystem::Register<AudioSource>();

        DE_LOG(LogReflection, Info, "Registered Engine Classes")

        // Initialize SDL - This is the foundation of the engine
        constexpr auto sdlInitFlags = SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO |
            SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS | SDL_INIT_SENSOR | SDL_INIT_CAMERA;
        if (!SDL_Init(sdlInitFlags))
        {
            std::string err = SDL_GetError();
            DE_LOG(Log, Critical, "SDL Init failed! SDL_Error: {}", err)
            throw std::runtime_error(err.c_str());
        }
        DE_LOG(Log, Trace, "SDL Init success")
    }

    void Engine::Initialize()
    {
        DE_LOG(LogEngine, Warn, "Engine Initializing")

        m_JobSubsystem = InitalizeSubsystem<JobSubsystem>();

        m_TimerSubsystem = InitalizeSubsystem<TimerSubsystem>();

        m_ProfileSubsystem = InitalizeSubsystem<ProfileSubsystem>();

        m_WindowSubsystem = InitalizeSubsystem<WindowSubsystem>();

        m_AudioSubsystem = InitalizeSubsystem<AudioSubsystem>();

        m_InputSubsystem = InitalizeSubsystem<InputSubsystem>();

        m_EventSubsystem = InitalizeSubsystem<EventSubsystem>();
        
        m_AssetSubsystem = InitalizeSubsystem<AssetSubsystem>();

        m_UISubsystem = InitalizeSubsystem<UISubsystem>();

        m_RendererSubsystem = InitalizeSubsystem<RendererSubsystem>();

        m_PhysicsSubsystem = InitalizeSubsystem<PhysicsSubsystem>();

        m_SceneSubsystem = InitalizeSubsystem<SceneSubsystem>();

        m_EditorSubsystem = InitalizeSubsystem<EditorSubsystem>();


        DE_LOG(LogEngine, Info, "Engine Initialized")
    }

    void Engine::Deinitialize()
    {
        DE_LOG(LogEngine, Trace, "Engine Deinitializing")

        SaveConfig();

        // Clear Core Dependencies
        //SDL_Quit();

        // Clear Subsystem pointers
        m_EditorSubsystem->Deinitialize();
        m_SceneSubsystem->Deinitialize();
        m_PhysicsSubsystem->Deinitialize();
        m_InputSubsystem->Deinitialize();
        m_EventSubsystem->Deinitialize();
        m_UISubsystem->Deinitialize();
        m_RendererSubsystem->Deinitialize();
        m_AssetSubsystem->Deinitialize();
        m_AudioSubsystem->Deinitialize();
        m_WindowSubsystem->Deinitialize();
        m_ProfileSubsystem->Deinitialize();
        m_FileSubsystem->Deinitialize();
        m_ReflectionSubsystem->Deinitialize();
        m_TimerSubsystem->Deinitialize();
        m_JobSubsystem->Deinitialize();
        s_Engine.reset();

        DE_LOG(LogEngine, Trace, "Engine Deinitialized")
        Logger::Deinitialize();
    }

    void Engine::LoadConfig()
    {
        try
        {
            // Load the config file. Exception will be thrown if the file doesn't exist so no need to check
            const YAML::Node& cfg = YAML::LoadFile(m_FileSubsystem->m_ProjectFile);
            if (!cfg.IsDefined())
            {
                DE_LOG(LogEngine, Error, "Failed to Load Engine Config: Invalid YAML")
                return;
            }

            // Project Name
            if (const YAML::Node& projectNameNode = cfg["Project Name"])
            {
                m_Config.ProjectName = projectNameNode.as<std::string>();
                DE_LOG(LogEngine, Info, "Loaded Engine Config: Project Name: {0}", m_Config.ProjectName)
            }
            
            // Startup scene
            if (const YAML::Node& startSceneNode = cfg["Startup Scene"])
            {
               m_Config.StartupScenePath = startSceneNode.as<std::string>();
            }
        }
        catch (const std::exception& e)
        {
            DE_LOG(LogEngine, Error, "Failed to Load Engine Config: {0}", e.what())
            DE_LOG(LogEngine, Info, "Generating New Engine Config")

            // Generate a new config file
            SaveConfig();
        }
    }

    void Engine::SaveConfig()
    {
        /*try
        {
            YAML::Emitter cfgEmitter;
            cfgEmitter << YAML::Comment("DENIX ENGINE CONFIGURATION");
            cfgEmitter << YAML::BeginMap;
            cfgEmitter << YAML::Key << "Startup Scene" << YAML::Value << (m_StartupScene
                                                                              ? m_StartupScene->GetAssetPath()
                                                                              : "");
            cfgEmitter << YAML::EndMap;

            if (FileSubsystem::WriteFile(m_EngineConfigPath, cfgEmitter.c_str()))
                DE_LOG(LogEngine, Info, "Saved Engine Config")
        }
        catch (const std::exception& e)
        {
            DE_LOG(LogEngine, Error, "Failed to Save Engine Config: {0}", e.what())

            // Do some error handling
            assert(false, "Failed to Save Engine Config");
        }*/
    }
}
