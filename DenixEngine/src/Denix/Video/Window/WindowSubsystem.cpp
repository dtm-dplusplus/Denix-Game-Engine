#include "WindowSubsystem.h"
#include "Denix/Video/GL/Viewport.h"

namespace Denix
{
    WindowSubsystem* WindowSubsystem::s_WindowSubSystem{ nullptr };

    void WindowSubsystem::Initialize()
    {
        Subsystem::Initialize();
        DE_LOG(LogWindow, Warn, "Initializing Window Subsystem")

        //Create window
        if (const Ref<SDL_GLWindow> window = MakeRef<SDL_GLWindow>())
        {
            if (!window->Initialize()) return;
            m_Window = window;
        }

        // Init Glew
        if (glewInit() != GLEW_OK)
        {
        	DE_LOG(LogWindow, Critical, "glewInit(): failed")
        		return;
        }
        DE_LOG(LogWindow, Trace, "glewInit(): success")
        
        m_DefaultViewport = MakeRef<Viewport>(m_Window->GetWidth(), m_Window->GetHeight());

        DE_LOG(LogWindow, Info, "Window Subsystem Initialized")
    }
}
