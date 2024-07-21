#include "depch.h"
#include "Window.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "GL/glew.h"
#include "imgui.h"
#include "Denix/Video/GL/Shader.h"

namespace Denix
{
	bool SDL_GLWindow::Initialize()
	{
		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) < 0)
		{
			// DE_LOG(Log, Critical, "SDL Init failed! SDL_Error: {}", SDL_GetError())
				return false;
		}
		DE_LOG(Log, Trace, "SDL Init success")

			// Set SDL OpenGL Version
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_GLMajorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_GLMinorVersion);
		
		// Set GL Attributes
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, m_GLDoubleBuffer);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_GLDepthSize);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, m_GLStencilSize);

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
			// DE_LOG(LogWindow, Critical, "SDL_GL_SetSwapInterval failed! SDL_Error: {}", SDL_GetError())
			return false;
		}

		// Set Viewport
		glViewport(0, 0, m_WinX, m_WinY);
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

	void SDL_GLWindow::WindowEvent(const SDL_Event* _event)
	{
		/*switch (_event->window)
		{
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED: // Additioanl CHeck e.window.windowID == SDL_GetWindowID(m_SDL_GLWindow)
		{
			m_IsOpen = false;
			DE_LOG(LogWindow, Trace, "Window Close Event")
		}
			break;

		case SDL_WINDOW_MINIMIZED:
		{
			DE_LOG(LogWindow, Trace, "Window Minimized Event")
		}
			break;

		case SDL_WINDOW_MAXIMIZED:
		{
			DE_LOG(LogWindow, Trace, "Window Maximized Event")
		}
			break;

		case SDL_EVENT_WINDOW_RESIZED:
		{
			m_WinX = _event->window.data1;
			m_WinY = _event->window.data2;
			glViewport(0, 0, m_WinX, m_WinY);

			DE_LOG(LogWindow, Trace, "Window Resized Event. Res: {}x{}", m_WinX, m_WinY)
		}
			break;

		default:	break;
		}*/
	}
}
