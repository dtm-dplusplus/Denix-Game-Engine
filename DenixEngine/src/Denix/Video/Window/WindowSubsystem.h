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
			s_WindowSubSystem = this;
		}

		~WindowSubsystem() override
		{
			s_WindowSubSystem = nullptr;
		}

		static WindowSubsystem* Get() { return s_WindowSubSystem; }

		void Initialize() override;

		void Deinitialize() override
		{
			m_Window->Deinitialize();

			SDL_Quit();

			DE_LOG(LogWindow, Trace, "Window Subsystem Deinitialized")
		}   

		static Ref<SDL_GLWindow> GetWindow() { return s_WindowSubSystem->m_Window; }
		static Ref<Viewport> GetDefaultViewport() { return s_WindowSubSystem->m_DefaultViewport; }
		/**
		 * 
		 * @return 
		 */
		static glm::vec2 GetWindowSize() { return s_WindowSubSystem->m_Window? s_WindowSubSystem->m_Window->GetWindowSize() : glm::vec2(0.0f); }
	private:
		static WindowSubsystem* s_WindowSubSystem;

		Ref<SDL_GLWindow> m_Window;


		Ref<Viewport> m_DefaultViewport;

		friend class Engine;
	};
}
