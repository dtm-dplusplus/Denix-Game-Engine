#include "Window.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "Denix/Scene/SceneSubsystem.h"

namespace Denix
{
    SDL_GLWindow::SDL_GLWindow(): m_SDL_GLWindow(nullptr), m_SDL_WindowFlags{SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY }, m_SDL_GLContext(nullptr)
    {
        // Set SDL OpenGL Version
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, NULL);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_GLMajorVersion);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_GLMinorVersion);

        // Set GL Attributes
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_GLDepthSize);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, m_GLStencilSize);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, m_GLDoubleBuffer);

        // Create SDL window
        m_SDL_GLWindow = SDL_CreateWindow(m_Title.c_str(), m_WinX, m_WinY,m_SDL_WindowFlags);

        //assert(m_SDL_GLWindow, "SDL_CreateWindow failed! SDL_Error: {}", SDL_GetError());
        
        
        // Create SDL GL Context
        m_SDL_GLContext = SDL_GL_CreateContext(m_SDL_GLWindow);
        if (!m_SDL_GLContext)
        {
            DE_LOG(LogWindow, Critical, "SDL_GL_CreateContext failed! SDL_Error: {}", SDL_GetError())
        }

        // Make current context
        if (!SDL_GL_MakeCurrent(m_SDL_GLWindow, m_SDL_GLContext))
        {
            // DE_LOG(LogWindow, Critical, "SDL_GL_MakeCurrent failed! SDL_Error: {}", SDL_GetError())
        }

        // Check if our attributes are set
        int major, minor;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
        if (!(major == m_GLMajorVersion && minor == m_GLMinorVersion))
        {
            DE_LOG(LogWindow, Warn, "SDL Requested OpenGL version did not match: {}.{} Got: {}.{}", m_GLMajorVersion, m_GLMinorVersion, major, minor)
        }

        
        // Enable Vsync
        if (!SDL_GL_SetSwapInterval(static_cast<int>(m_VsyncMode)))
        {
            DE_LOG(LogWindow, Critical, "SDL_GL_SetSwapInterval failed! SDL_Error: {}", SDL_GetError())
        }

        // Enable Depth Test
        glEnable(GL_DEPTH_TEST);

        // Set boolean to notify systems of successful window creation
        m_IsOpen = true;

        DE_LOG(LogWindow, Trace, "Created Window: {} Res: {}x{}", m_Title, m_WinX, m_WinY)
    }

    SDL_GLWindow::~SDL_GLWindow()
    {
        // Destroys window and context
        SDL_GL_DestroyContext(SDL_GL_GetCurrentContext());
        SDL_DestroyWindow(m_SDL_GLWindow);
        DE_LOG(LogWindow, Trace, "Destroyed Window")
    }

    void SDL_GLWindow::ClearBuffer()
    {
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void SDL_GLWindow::SwapBuffers()
    {
        SDL_GL_SwapWindow(m_SDL_GLWindow);
    }
}
