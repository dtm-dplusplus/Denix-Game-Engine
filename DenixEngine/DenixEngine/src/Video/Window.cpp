#include "DePch.h"
#include "Window.h"

#include "SDL.h"
#include "GL/glew.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

namespace Denix
{
	bool SDL_GLWindow::Initialize()
	{
		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0)
		{
			DE_LOG(Log, Critical, "SDL Init failed! SDL_Error: {}", SDL_GetError())
				return false;
		}
		DE_LOG(Log, Trace, "SDL Init success")

			// Set SDL OpenGL Version
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_GLMajorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_GLMinorVersion);

		// Useful for virtual keyboards, complex chararacter sets
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

		// Set GL Attributes
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, m_GLDoubleBuffer);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_GLDepthSize);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, m_GLStencilSize);

		// Create SDL window
		if (SDL_Window* window = SDL_CreateWindow(m_Title.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			m_WinX, m_WinY, m_SDL_WindowFlags))
		{
			m_SDL_GLWindow = window;
			DE_LOG(LogWindow, Trace, "SDL_CreateWindow success")
		}
		else
		{
			DE_LOG(LogWindow, Critical, "SDL_CreateWindow failed! SDL_Error: {}", SDL_GetError())
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
			DE_LOG(LogWindow, Critical, "SDL_GL_CreateContext failed! SDL_Error: {}", SDL_GetError())
			return false;
		}

		// Make current context
		if(SDL_GL_MakeCurrent(m_SDL_GLWindow, m_SDL_GLContext) < 0)
		{
			DE_LOG(LogWindow, Critical, "SDL_GL_MakeCurrent failed! SDL_Error: {}", SDL_GetError())
			return false;
		}

		// Enable Vsync
		if(SDL_GL_SetSwapInterval(static_cast<int>(m_VsyncMode)) < 0)
		{
			DE_LOG(LogWindow, Critical, "SDL_GL_SetSwapInterval failed! SDL_Error: {}", SDL_GetError())
			return false;
		}

		// Set Viewport
		glViewport(0, 0, m_WinX, m_WinY);

		m_IsOpen = true;

		DE_LOG(LogWindow, Trace, "Created Window: {} Res: {}x{}", m_Title, m_WinX, m_WinY)

		return true;
	}

	void SDL_GLWindow::Deinitialize()
	{
		// Destroys window and context
		SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
		SDL_DestroyWindow(m_SDL_GLWindow);
		DE_LOG(LogWindow, Trace, "Destroyed Window")
	}

	void SDL_GLWindow::ClearBuffer()
	{
		// Clear buffer, move to renderer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// New ImGui frame - Move to UI class
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void SDL_GLWindow::SwapBuffers()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(m_SDL_GLWindow);
	}

	void SDL_GLWindow::PollEvents()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			ImGui_ImplSDL2_ProcessEvent(&e);

			if (e.type == SDL_WINDOWEVENT)
			{
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE: // Additioanl CHeck e.window.windowID == SDL_GetWindowID(m_SDL_GLWindow)
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

				case SDL_WINDOWEVENT_RESIZED:
					{
						m_WinX = e.window.data1;
						m_WinY = e.window.data2;
						glViewport(0, 0, m_WinX, m_WinY);

						DE_LOG(LogWindow, Trace, "Window Resized Event. Res: {}x{}", m_WinX, m_WinY)
					}
					break;

				default : break;
				}
			}
		}
	}
}
