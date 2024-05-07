#pragma once

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"
#include "Denix/Video/Window/Window.h"
#include <SDL.h>
#include <GL/glew.h>

namespace Denix
{
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

		void Initialize() override
		{
			//Create window
			if(const Ref<SDL_GLWindow> window = MakeRef<SDL_GLWindow>())
			{
				if(!window->Initialize()) return;
				m_Window = window;
			}
		
			// Init Glew
			if (glewInit() != GLEW_OK)
			{
				DE_LOG(Log, Critical, "glewInit(): failed")
				return;
			}
			DE_LOG(Log, Trace, "glewInit(): success")

			DE_LOG(LogWindow, Trace, "Window Subsystem Initialized")

			m_Initialized = true;
		}

		void Deinitialize() override
		{
			m_Window->Deinitialize();

			SDL_Quit();

			DE_LOG(LogWindow, Trace, "Window Subsystem Deinitialized")

			m_Initialized = false;
		}   

		static Ref<SDL_GLWindow> GetWindow() { return s_WindowSubSystem->m_Window; }

	private:
		Ref<SDL_GLWindow> m_Window;

		static WindowSubsystem* s_WindowSubSystem;


		friend class Engine;
	};
}
