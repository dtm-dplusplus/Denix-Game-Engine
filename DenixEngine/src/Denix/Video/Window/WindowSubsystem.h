#pragma once

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"
#include "Denix/Video/Window/Window.h"
#include <SDL3/SDL.h>

#include <GL/glew.h>

namespace Denix
{
	class Viewport;

	class WindowSubsystem: public Subsystem
	{
	public:
		WindowSubsystem()
		{
			s_WindowSubsystem = this;

			DE_LOG_CREATE(LogGL)
			DE_LOG_CREATE(LogWindow)
			DE_LOG_CREATE(LogShader)
		}

		~WindowSubsystem() override
		{
			s_WindowSubsystem = nullptr;
		}

		static void ToggleFullscreen();

		static WindowSubsystem* Get() { return s_WindowSubsystem; }

		void Initialize() override;

		void Deinitialize() override
		{
			m_Window->Deinitialize();

			SDL_Quit();

			DE_LOG(LogWindow, Trace, "Window Subsystem Deinitialized")
		}   

		static Ref<SDL_GLWindow> GetWindow() { return s_WindowSubsystem->m_Window; }
		static Ref<Viewport> GetDefaultViewport() { return s_WindowSubsystem->m_DefaultViewport; }
		/**
		 * 
		 * @return 
		 */
		static glm::vec2 GetWindowSize() { return s_WindowSubsystem->m_Window? s_WindowSubsystem->m_Window->GetWindowSize() : glm::vec2(0.0f); }
	private:
		static WindowSubsystem* s_WindowSubsystem;

		Ref<SDL_GLWindow> m_Window;


		Ref<Viewport> m_DefaultViewport;

		friend class Engine;
	};
}
