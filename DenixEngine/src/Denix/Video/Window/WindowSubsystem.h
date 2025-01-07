#pragma once

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"
#include "Denix/Video/Window/Window.h"
#include <SDL3/SDL.h>

#include <GL/glew.h>

namespace Denix
{
	class Viewport;

	class WindowSubsystem: public Subsystem<WindowSubsystem>
	{
	public:
		WindowSubsystem() = default;

		~WindowSubsystem() override = default;

		static void ToggleFullscreen();

		static Ref<SDL_GLWindow> GetWindow() { return s_Instance->m_Window; }
		static Ref<Viewport> GetDefaultViewport() { return s_Instance->m_DefaultViewport; }
		
		static glm::vec2 GetWindowSize() { return s_Instance->m_Window->GetWindowSize(); }
		static int GetWindowWidth() { return s_Instance->m_Window->GetWidth(); }
		static int GetWindowHeight() { return s_Instance->m_Window->GetHeight(); }
		
	private:
		void Initialize() override;

		void Deinitialize() override
		{
			m_Window.reset();


			DE_LOG(LogWindow, Trace, "Window Subsystem Deinitialized")

			Subsystem::Deinitialize();
		}   

		Ref<SDL_GLWindow> m_Window;

		Ref<Viewport> m_DefaultViewport;

		friend class Engine;
	};
}
