#include "Window.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "Denix/Scene/SceneSubsystem.h"

//#include "Denix/Scene/SceneSubsystem.h"
//#include "GL/glew.h"
//#include "imgui.h"
//#include "Denix/Video/GL/Shader.h"

namespace Denix
{
    bool SDL_GLWindow::Initialize()
    {
        // Create SDL window
        if (SDL_Window* window = SDL_CreateWindow(m_Title.c_str(), m_WinX, m_WinY,
                                                  m_SDL_WindowFlags))
        {
            m_SDL_GLWindow = window;
            DE_LOG(LogWindow, Trace, "SDL_CreateWindow success")
        }
        else
        {
            //DE_LOG(LogWindow, Critical, "SDL_CreateWindow failed! SDL_Error: {}", SDL_GetError())
            return false;
        }

        // Create SDL GL Context
        if (const SDL_GLContext context = SDL_GL_CreateContext(m_SDL_GLWindow))
        {
            m_SDL_GLContext = context;
            DE_LOG(LogWindow, Trace, "SDL_GL_CreateContext success")
        }
        else
        {
            // DE_LOG(LogWindow, Critical, "SDL_GL_CreateContext failed! SDL_Error: {}", SDL_GetError())
            return false;
        }

        // Make current context
        if (SDL_GL_MakeCurrent(m_SDL_GLWindow, m_SDL_GLContext) < 0)
        {
            // DE_LOG(LogWindow, Critical, "SDL_GL_MakeCurrent failed! SDL_Error: {}", SDL_GetError())
            return false;
        }

        // Enable Vsync
        if (SDL_GL_SetSwapInterval(static_cast<int>(m_VsyncMode)) < 0)
        {
             DE_LOG(LogWindow, Critical, "SDL_GL_SetSwapInterval failed! SDL_Error: {}", SDL_GetError())
            return false;
        }

        // Enable Depth Test
        glEnable(GL_DEPTH_TEST);
        m_IsOpen = true;

        DE_LOG(LogWindow, Trace, "Created Window: {} Res: {}x{}", m_Title, m_WinX, m_WinY)

        return true;
    }

    void SDL_GLWindow::Deinitialize()
    {
        // Destroys window and context
        SDL_GL_DestroyContext(SDL_GL_GetCurrentContext());
        SDL_DestroyWindow(m_SDL_GLWindow);
        DE_LOG(LogWindow, Trace, "Destroyed Window")
    }

    void SDL_GLWindow::ClearBuffer()
    {
        // Clear buffer, move to renderer
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void SDL_GLWindow::SwapBuffers()
    {
        SDL_GL_SwapWindow(m_SDL_GLWindow);
    }
}
