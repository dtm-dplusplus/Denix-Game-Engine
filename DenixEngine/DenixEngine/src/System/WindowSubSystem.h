#pragma once

#include "Core.h"
#include "SubSystem.h"
#include "Video/Window.h"
#include <SDL.h>
#include <GL/glew.h>

namespace Denix
{
	class WindowSubSystem: public SubSystem
	{
	public:
		WindowSubSystem()
		{
			s_WindowSubSystem = this;
		}

		~WindowSubSystem() override
		{
			s_WindowSubSystem = nullptr;
		}

		static WindowSubSystem* Get() { return s_WindowSubSystem; }

		void Initialize() override
		{
			//Create window
			if(const Ref<SDL_GLWindow> window = std::make_shared<SDL_GLWindow>())
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

		Ref<SDL_GLWindow> GetWindow() const { return m_Window; }

	private:
		Ref<SDL_GLWindow> m_Window;

		static WindowSubSystem* s_WindowSubSystem;


		friend class Engine;
	};
}
