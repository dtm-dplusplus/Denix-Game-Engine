#include "Engine.h"

#include "Denix/System/SubSystem.h"
#include "Denix/Thread/JobSubsystem.h"
#include "Denix/Reflection/ReflectionSubsystem.h"
#include "Denix/Video/Window/WindowSubsystem.h"
#include "Denix/UI/UISubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Video/Renderer/RendererSubsystem.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Core/FileSubsystem.h"
#include "Denix/Core/TimerSubsystem.h"
#include "Profile/ProfileSubsystem.h"

namespace Denix
{
	Engine* Engine::s_Engine{nullptr};

	Engine::Engine()
	{
		s_Engine = this;
		m_StartupScene = nullptr;
		Logger::Initialize();

		// We initialize the thread subsystem here because it is used to create the other subsystems
		m_JobSubsystem = InitalizeSubsystem<JobSubsystem>();
		
		// We initialize the reflection subsystem here because it is used by the client engine constructor
		// Register all classes that need to be reflected here. This will be moved to some kind of pre build event & parser in the future
		m_ReflectionSubsystem = InitalizeSubsystem<ReflectionSubsystem>();

		// Register classes
		ReflectionSubsystem::Register<BaseObject>();
		ReflectionSubsystem::Register<Actor>();
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

		ReflectionSubsystem::Register<Light>();
		ReflectionSubsystem::Register<DirectionalLight>();
		ReflectionSubsystem::Register<PointLight>();
		ReflectionSubsystem::Register<SpotLight>();

		ReflectionSubsystem::Register<Camera>();
	}

	Engine::~Engine()
	{
		s_Engine = nullptr;

		Logger::Deinitialize();
	}

	void Engine::Initialize()
	{
		DE_LOG(LogEngine, Warn, "Engine Initializing")

		m_TimerSubsystem = InitalizeSubsystem<TimerSubsystem>();

		m_ProfileSubsystem = InitalizeSubsystem<ProfileSubsystem>();
		
		m_FileSubsystem = InitalizeSubsystem<FileSubsystem>(m_ProjectName);

		m_WindowSubsystem = InitalizeSubsystem<WindowSubsystem>();

		m_ResourceSubsystem = InitalizeSubsystem<ResourceSubsystem>();

		// set the engine config path
		m_EngineConfigPath = FileSubsystem::GetProjectRoot() + "Config\\Engine.cfg";
		
		// Load the engine config. This will set the startup scene and validate the engine config
		LoadConfig();

		m_RendererSubsystem = InitalizeSubsystem<RendererSubsystem>();

		m_UISubsystem = InitalizeSubsystem<UISubsystem>();

		m_PhysicsSubsystem = InitalizeSubsystem<PhysicsSubsystem>();

		m_InputSubsystem = InitalizeSubsystem<InputSubsystem>();

		m_SceneSubsystem = InitalizeSubsystem<SceneSubsystem>(m_StartupScene);

		m_EditorSubsystem = InitalizeSubsystem<EditorSubsystem>();


		DE_LOG(LogEngine, Info, "Engine Initialized")
}

	void Engine::Deinitialize()
	{
		DE_LOG(LogEngine, Trace, "Engine Deinitializing")

		// Deinitialie SubSystems in the reverse order of initialization
		for (const auto& subsystem : std::views::reverse(m_Subsystems))
		{
			subsystem->Deinitialize();
		}

		DE_LOG(LogEngine, Trace, "Engine Deinitialized")
	}

	void Engine::Run()
	{
		Initialize();

		// Engine Loop
		while(m_WindowSubsystem->m_Window->IsOpen())
		{
			// Setup timer system for the new frame.
			m_TimerSubsystem->BeginFrame();
			m_ProfileSubsystem->Update(m_TimerSubsystem->m_FrameTime);
			
			// Poll input & Events. Events will be dispatched to the appropriate subsystems
			DE_PROFILE(Input Poll)
			m_InputSubsystem->Poll();
			DE_PROFILE_END(Input Poll)

			
			// Clear the offscreen frame buffer
			DE_PROFILE(New Window Buffer)
			m_UISubsystem->NewFrame();
			m_WindowSubsystem->m_Window->ClearBuffer();
			m_SceneSubsystem->m_ActiveScene->m_ActiveCamera->m_Viewport->m_FrameBuffer->Bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			DE_PROFILE_END(New Window Buffer)
			
			
			// Prepare physics system for scene update
			DE_PROFILE(Physics PreUpdate)
			m_PhysicsSubsystem->PreUpdate(m_TimerSubsystem->m_DeltaTime);
			DE_PROFILE_END(Physics PreUpdate)

			// Job Subsystem Test
			// UI & Editor should be part of the same job group. Editor relies on UI to be updated first.
			// Therefore we must WaitForCounter individually to ensure these two jobs are executed serially.
			// We will build a  job builder which can queue these jobs and ensure they run serially
			// Currently, jobs are added to the job subsystem and executed instantly. In the future they should be scheduled in some kind of group/bucket
			// Update the UI & Editor for any changes
			DE_PROFILE(Editor Update)
			Ref<Counter> uiCounter = MakeRef<Counter>(1);
			m_JobSubsystem->AddJob("UI Update", Priority::NORMAL, uiCounter, &UISubsystem::Update, m_UISubsystem.get(), m_TimerSubsystem->m_DeltaTime);
			WaitForCounter(*uiCounter);
			
			Ref<Counter> editorCounter = MakeRef<Counter>(1);
			m_JobSubsystem->AddJob("Editor Update", Priority::NORMAL, editorCounter, &EditorSubsystem::Update, m_EditorSubsystem.get(), m_TimerSubsystem->m_DeltaTime);
			WaitForCounter(*editorCounter);
			DE_PROFILE_END(Editor Update)


			// Render Parallel test
			// Rendering has to run on main thread as opengl context is thread specific. We will run the scene & physics update in parallel instead
			Ref<Counter> sceneCounter;
			if (m_ParallelRendering)
			{
				sceneCounter = MakeRef<Counter>(1);

				// Update the scene. The majority of the client game logic will be here
				m_JobSubsystem->AddJob("Scene Update", Priority::NORMAL, sceneCounter, &SceneSubsystem::Update, m_SceneSubsystem.get(), m_TimerSubsystem->m_DeltaTime);

				// Update the physics system. Collision detection and resolution will be here
				//m_JobSubsystem->AddJob("Physics Update", Priority::NORMAL, sceneCounter, &PhysicsSubsystem::Update, m_PhysicsSubsystem.get(), m_TimerSubsystem->m_DeltaTime);
			}
			else
			{
				// Update the scene. The majority of the client game logic will be here
				m_SceneSubsystem->Update(m_TimerSubsystem->m_DeltaTime);

				// Update the physics system. Collision detection and resolution will be here
				//m_PhysicsSubsystem->Update(m_TimerSubsystem->m_DeltaTime);
			}

			// Render the scene. This runs on the main thread as it requires the opengl context
			m_RendererSubsystem->RenderScene();

			// Wait for the scene & physics update to complete
			if (m_ParallelRendering) WaitForCounter(*sceneCounter);
			
			// Unbind from the viewport framebuffer & Draw the framebuffer texture to the default screen buffer
			DE_PROFILE(Draw Viewport)
			FrameBuffer::Unbind();
			m_SceneSubsystem->m_ActiveScene->m_ActiveCamera->m_Viewport->DrawViewport();
			m_UISubsystem->RenderUI(); // Swap buffers and render UI
			m_WindowSubsystem->m_Window->SwapBuffers();
			m_UISubsystem->ViewportUpdate(m_WindowSubsystem->m_Window);
			DE_PROFILE_END(Draw Viewport)
			
			// Run the garbage collector
			DE_PROFILE(Clean Rubbish)
			m_SceneSubsystem->CleanRubbish();
			DE_PROFILE_END(Clean Rubbish)

			
			m_TimerSubsystem->EndFrame();
		}
		
		Deinitialize();
	}

	void Engine::LoadConfig()
	{
		try
		{
			// Load the config file. Exception will be thrown if the file doesn't exist so no need to check
			const YAML::Node& cfg = YAML::LoadFile(m_EngineConfigPath);

			// Validate startup scene
			if(const YAML::Node& startSceneNode = cfg["Startup Scene"])
			{
				if(Ref<Asset> startSceneAsset = ResourceSubsystem::GetSceneAsset(startSceneNode.as<std::string>()))
				{
					m_StartupScene = startSceneAsset;
					DE_LOG(LogEngine, Info, "Loaded Engine Config: Startup Scene: {0}", startSceneAsset->GetAssetName())
				}
				else
				{
					DE_LOG(LogEngine, Warn, "Load Engine Config: Startup Scene Not Found")
				}
			}
		}
		catch(const std::exception& e)
		{
			DE_LOG(LogEngine, Error, "Failed to Load Engine Config: {0}", e.what())
			DE_LOG(LogEngine, Info, "Generating New Engine Config")
			
			// Generate a new config file
			SaveConfig();
		}
		
	}

	void Engine::SaveConfig()
	{
		try
		{
				
			YAML::Emitter cfgEmitter;
			cfgEmitter << YAML::Comment("DENIX ENGINE CONFIGURATION");
			cfgEmitter << YAML::BeginMap;
			cfgEmitter << YAML::Key << "Startup Scene" << YAML::Value << (m_StartupScene? m_StartupScene->GetAssetPath() : "");
			cfgEmitter << YAML::EndMap;
			
			if(FileSubsystem::WriteFile(m_EngineConfigPath, cfgEmitter.c_str()))
				DE_LOG(LogEngine, Info, "Saved Engine Config")
		}
		catch(const std::exception& e)
		{
			DE_LOG(LogEngine, Error, "Failed to Save Engine Config: {0}", e.what())

			// Do some error handling
			assert(false, "Failed to Save Engine Config");
		}
	}

	Ref<Asset> Engine::GetStartupScene() const
	{
		return m_StartupScene;
	}

	void Engine::SetStartupScene(const Ref<Asset>& _ref)
	{
		if(_ref)
		{
			s_Engine->m_StartupScene = _ref;
			DE_LOG(LogEngine, Info, "Set Startup Scene: {0}", _ref->GetAssetPath())
			s_Engine->SaveConfig();
		}
	}
}
