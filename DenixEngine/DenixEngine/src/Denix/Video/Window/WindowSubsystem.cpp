#include "WindowSubsystem.h"
#include "Denix/Video/GL/Viewport.h"

namespace Denix
{
	WindowSubsystem* WindowSubsystem::s_WindowSubSystem{ nullptr };

	void WindowSubsystem::Initialize()
	{
		//Create window
		if (const Ref<SDL_GLWindow> window = MakeRef<SDL_GLWindow>())
		{
			if (!window->Initialize()) return;
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
		
		m_DefaultViewport = MakeRef<Viewport>(m_Window->GetWidth(), m_Window->GetHeight());

		m_Initialized = true;
	}
}
