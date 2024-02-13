#include "DePch.h"
#include "Window.h"

#include "SDL.h"
#include "SDL_video.h"
#include "GL/glew.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"


bool Window::Start(const SDL_WindowFlags _flags)
{
	SDL_WindowFlags flags = (SDL_WindowFlags)(_flags | SDL_WINDOW_OPENGL);

	m_SDL_Window = SDL_CreateWindow("Denix Engine",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_WinX, m_WinY, flags);

	if (!m_SDL_Window)
	{
		DE_LOG(LogWindow, Critical, "SDL_CreateWindow failed! SDL_Error: {}", SDL_GetError())
		return false;
	}
	DE_LOG(LogWindow, Trace, "SDL_CreateWindow success")

	// Create Context
	if (!SDL_GL_CreateContext(m_SDL_Window))
	{
		DE_LOG(LogWindow, Critical, "SDL_GL_CreateContext failed! SDL_Error: {}", SDL_GetError())
		return false;
	}
	DE_LOG(LogWindow, Trace, "SDL_GL_CreateContext success")

	if(SDL_GL_MakeCurrent(m_SDL_Window, SDL_GL_GetCurrentContext()) < 0)
	{
		DE_LOG(LogWindow, Critical, "SDL_GL_MakeCurrent failed! SDL_Error: {}", SDL_GetError())
		return false;
	}

	if(SDL_GL_SetSwapInterval(1) < 0)
	{
		DE_LOG(LogWindow, Critical, "SDL_GL_SetSwapInterval failed! SDL_Error: {}", SDL_GetError())
		return false;
	}

	m_IsOpen = true;

	DE_LOG(LogWindow, Info, "Create Window: {} success!", m_Title)

	return true;
}

void Window::Stop()
{
	// Destroys window and context
	SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
	SDL_DestroyWindow(m_SDL_Window);
	DE_LOG(LogWindow, Trace, "Destroyed Window")
}

void Window::ClearBuffer()
{
	// Clear buffer, move to renderer
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// New ImGui frame - Move to UI class
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void Window::SwapBuffers()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(m_SDL_Window);
}

void Window::PollEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE
			&& e.window.windowID == SDL_GetWindowID(m_SDL_Window))
			m_IsOpen = false;
	}
}