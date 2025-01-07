#include "WindowSubsystem.h"
#include "Denix/Video/GL/Viewport.h"

namespace Denix
{
    void WindowSubsystem::ToggleFullscreen()
    {
        s_Instance->m_Window->ToggleFullscreen();
    }

    void WindowSubsystem::Initialize()
    {
        Subsystem::Initialize();
        DE_LOG(LogWindow, Warn, "Initializing Window Subsystem")

        SDL_GLWindow::m_GLMajorVersion = 3;
        SDL_GLWindow::m_GLMinorVersion = 0;
        SDL_GLWindow::m_GLDepthSize = 24;
        SDL_GLWindow::m_GLStencilSize = 8;
        SDL_GLWindow::m_GLDoubleBuffer = 1;
        
        
        
        
        //Create main window
        m_Window = MakeRef<SDL_GLWindow>();
        if (!m_Window->m_IsOpen)
        {
            const std::string error = "Failed to create main window";
            DE_LOG(LogWindow, Critical, error)
            throw std::runtime_error(error.c_str());
        }
        
        // Init Glew
        if (glewInit() != GLEW_OK)
        {
        	DE_LOG(LogWindow, Critical, "glewInit(): failed")
        		return;
        }
        DE_LOG(LogWindow, Trace, "glewInit(): success")
        
        DE_LOG(LogWindow, Info, "Window Subsystem Initialized")
    }

    void WindowSubsystem::Deinitialize()
    {
        m_Window.reset();


        DE_LOG(LogWindow, Trace, "Window Subsystem Deinitialized")

        Subsystem::Deinitialize();
    }
}
