#include "Engine.h"

#include "Denix/System/SubSystem.h"
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

namespace Denix
{
	Engine* Engine::s_Engine{nullptr};

	Engine::Engine()
	{
		s_Engine = this;

		Logger::Initialize();

		// We initialize the reflection subsystem here because it is used by the client engine constructor
		m_ReflectionSubsystem = InitalizeSubsystem<ReflectionSubsystem>();
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

		m_FileSubsystem = InitalizeSubsystem<FileSubsystem>(m_ProjectName);

		m_WindowSubsystem = InitalizeSubsystem<WindowSubsystem>();

		m_RendererSubsystem = InitalizeSubsystem<RendererSubsystem>();

		m_ResourceSubsystem = InitalizeSubsystem<ResourceSubsystem>();

		m_UISubsystem = InitalizeSubsystem<UISubsystem>();


		m_PhysicsSubsystem = InitalizeSubsystem<PhysicsSubsystem>();



		m_InputSubsystem = InitalizeSubsystem<InputSubsystem>();


		m_SceneSubsystem = InitalizeSubsystem<SceneSubsystem>();

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
			m_TimerSubsystem->BeginFrame();

			m_InputSubsystem->Poll();

			m_UISubsystem->NewFrame();
			m_WindowSubsystem->m_Window->ClearBuffer();

			// Bind viewport framebuffer
			m_SceneSubsystem->m_ActiveScene->m_ActiveCamera->m_Viewport->m_FrameBuffer->Bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			m_PhysicsSubsystem->PreUpdate(m_TimerSubsystem->m_DeltaTime);
			
			m_UISubsystem->Update(m_TimerSubsystem->m_DeltaTime);
			
			m_EditorSubsystem->Update(m_TimerSubsystem->m_DeltaTime);
			
			m_SceneSubsystem->Update(m_TimerSubsystem->m_DeltaTime);
			
			m_PhysicsSubsystem->Update(m_TimerSubsystem->m_DeltaTime);

			// Draw to viewport framebuffer
			m_RendererSubsystem->RenderScene();
			FrameBuffer::Unbind();
			
			// Draw the framebuffer texture to the default screen buffer
			m_SceneSubsystem->m_ActiveScene->m_ActiveCamera->m_Viewport->DrawViewport();

			// Swap buffers and render UI
			m_UISubsystem->RenderUI();
			m_WindowSubsystem->m_Window->SwapBuffers();
			m_UISubsystem->ViewportUpdate(m_WindowSubsystem->m_Window);

			// Run the garbage collector
			m_SceneSubsystem->CleanRubbish();

			m_TimerSubsystem->EndFrame();
		}
		
		Deinitialize();
	}

	void Engine::LoadConfig()
	{
	}

	void Engine::SaveConfig()
	{
	}
}
