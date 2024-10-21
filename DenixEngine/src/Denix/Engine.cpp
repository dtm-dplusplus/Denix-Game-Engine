#include "depch.h"
#include "Engine.h"


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

namespace Denix
{
	Engine* Engine::s_Engine{nullptr};

	Engine::Engine()
	{
		s_Engine = this;
	}

	Engine::~Engine()
	{
		s_Engine = nullptr;
	}

	void Engine::Initialize()
	{
		DE_LOG(LogEngine, Trace, "Engine Starting up")

		PreInitialize();

		m_TimerSubSystem = new TimerSubsystem;
		m_Subsystems.push_back(m_TimerSubSystem);

		m_FileSubSystem = new FileSubsystem(m_ProjectName);
		m_FileSubSystem->m_ProjectName = m_ProjectName;
		m_Subsystems.push_back(m_FileSubSystem);

		m_WindowSubsystem = new WindowSubsystem;
		m_Subsystems.push_back(m_WindowSubsystem);

		m_RendererSubSystem = new RendererSubsystem;
		m_Subsystems.push_back(m_RendererSubSystem);

		m_ResourceSubSystem = new ResourceSubsystem;
		m_Subsystems.push_back(m_ResourceSubSystem);

		m_UISubsystem = new UISubsystem;
		m_Subsystems.push_back(m_UISubsystem);

		m_EditorSubSystem = new EditorSubsystem;
		m_Subsystems.push_back(m_EditorSubSystem);

		m_SceneSubSystem = new SceneSubsystem;
		m_Subsystems.push_back(m_SceneSubSystem);

		m_PhysicsSubSystem = new PhysicsSubsystem;
		m_Subsystems.push_back(m_PhysicsSubSystem);

		m_InputSubsystem = new InputSubsystem;
		m_Subsystems.push_back(m_InputSubsystem);

	    // Order of initialization is defined above
		for(const auto& subsystem : m_Subsystems)
		{
			try
			{
				subsystem->Initialize();
			}
			catch (const std::exception& e)
			{
				// Assert and terminate
				DE_LOG(LogEngine, Critical, "Failed to Initialize Subsystem: {0}", e.what())
				assert(false, e.what());
			}
		}
			

		DE_LOG(LogEngine, Info, "Engine Initialized")

		PostInitialize();
	}

	void Engine::Deinitialize()
	{
		DE_LOG(LogEngine, Trace, "Engine Shutting Down")

		// Deinitialie SubSystems in the reverse order of initialization
		for (const auto& subsystem : std::views::reverse(m_Subsystems))
		{
			subsystem->Deinitialize();
			delete subsystem;
		}

		DE_LOG(LogEngine, Trace, "Engine Deinitialized")
	}

	void Engine::Run()
	{
		Initialize();

		// Tempory Fix until Reflection  is built. Passes the derived scene class to the scene subsystem
		PostInitialize();

		// Engine Loop
		while(m_WindowSubsystem->m_Window->IsOpen())
		{
			m_TimerSubSystem->BeginFrame();

			m_InputSubsystem->Poll();

			m_UISubsystem->NewFrame();
			m_WindowSubsystem->m_Window->ClearBuffer();

			// Bind viewport framebuffer
			m_SceneSubSystem->m_ActiveScene->m_ActiveCamera->m_Viewport->m_FrameBuffer->Bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			m_PhysicsSubSystem->PreUpdate(m_TimerSubSystem->m_DeltaTime);
			
			m_UISubsystem->Update(m_TimerSubSystem->m_DeltaTime);
			
			m_EditorSubSystem->Update(m_TimerSubSystem->m_DeltaTime);
			
			m_SceneSubSystem->Update(m_TimerSubSystem->m_DeltaTime);
			
			m_PhysicsSubSystem->Update(m_TimerSubSystem->m_DeltaTime);

			// Draw to viewport framebuffer
			m_RendererSubSystem->RenderScene();
			FrameBuffer::Unbind();
			
			// Draw the framebuffer texture to the default screen buffer
			m_SceneSubSystem->m_ActiveScene->m_ActiveCamera->m_Viewport->DrawViewport();

			// Swap buffers and render UI
			m_UISubsystem->RenderUI();
			m_WindowSubsystem->m_Window->SwapBuffers();
			m_UISubsystem->ViewportUpdate(m_WindowSubsystem->m_Window);

			// Run the garbage collector
			m_SceneSubSystem->CleanRubbish();

			m_TimerSubSystem->EndFrame();
		}
		
		Deinitialize();
	}

	void Engine::LoadConfig()
	{
	}

	void Engine::SaveConfig()
	{
	}

	void Engine::PreInitialize()
	{
		DE_LOG(LogEngine, Trace, "Engine Pre-Initialized")

	}

	void Engine::PostInitialize()
	{
		DE_LOG(LogEngine, Trace, "Engine Post-Initialized")
	}


}
