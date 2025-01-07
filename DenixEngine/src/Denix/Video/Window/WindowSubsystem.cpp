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
        
        // Set SDL OpenGL Version
       SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, SDL_GLWindow::m_GLMajorVersion);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, SDL_GLWindow::m_GLMinorVersion);

        // Set GL Attributes
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, SDL_GLWindow::m_GLDepthSize);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, SDL_GLWindow::m_GLStencilSize);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_GLWindow::m_GLDoubleBuffer);
        
        
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
        
        DE_LOG(LogWindow, Info, "Window Subsystem Initialized")
    }
}
