#pragma once

#include <SDL_events.h>

#include "Denix/Core.h"
#include "Denix/Core/Math.h"

#include "SDL_video.h"
#include "GL/glew.h"

namespace Denix
{
	class Window
	{
	public:
		Window() : m_IsOpen{ false }, m_Title{ "Denix Engine" }, m_WinX{ 1920 }, m_WinY{ 1080 } {}

		virtual ~Window() = default;

		virtual bool Initialize() = 0;
		virtual void Deinitialize() = 0;

		virtual void ClearBuffer() = 0;
		virtual void SwapBuffers() = 0;

		virtual void WindowEvent(const SDL_Event* _event) {}

		bool IsOpen() const { return m_IsOpen; }
		bool IsFullscreen() const { return m_IsFullscreen; }

		glm::vec2 GetWindowSize() const { return { m_WinX, m_WinY }; }

		int GetWidth() const { return m_WinX; }
		int GetHeight() const { return m_WinY; }

		std::string GetTitle() const { return m_Title; }

		glm::vec4 GetClearColor() const { return m_ClearColor; }
		glm::vec4& GetClearColor() { return m_ClearColor; }
		void SetClearColor(const glm::vec4& _color) { m_ClearColor = _color; }

	protected:
		bool m_IsOpen;
		bool m_IsFullscreen = false;

		std::string m_Title;

		int m_WinX;
		int m_WinY;

		glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		friend class WindowSubsystem;
	};


	class SDL_GLWindow final : public Window
	{
	public:
		SDL_GLWindow() : m_SDL_GLWindow(nullptr), m_SDL_GLContext(nullptr)
		{
			m_SDL_WindowFlags = static_cast<SDL_WindowFlags>(
				SDL_WINDOW_OPENGL |
				SDL_WINDOW_MAXIMIZED |
				SDL_WINDOW_RESIZABLE |
				SDL_WINDOW_ALLOW_HIGHDPI
				);

		}

		~SDL_GLWindow() override = default;

		void WindowEvent(const SDL_Event* _event) override;


		void ToggleFullscreen()
		{
			if (m_IsFullscreen)
			{
				SDL_SetWindowFullscreen(m_SDL_GLWindow, SDL_WINDOW_MAXIMIZED);
				m_IsFullscreen = false;
			}
			else
			{
				SDL_SetWindowFullscreen(m_SDL_GLWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
				m_IsFullscreen = true;
			}
			DE_LOG(LogWindow, Info, "Toggled Fullscreen: {}", m_IsFullscreen);
		}

		void RequestClose()
		{
			m_IsOpen = false;
		}

		enum class SDL_GLVsyncMode
		{
			Off = 0,
			On = 1,
			Adaptive = -1
		};

		SDL_Window* GetSDLWindow() const { return m_SDL_GLWindow; }

		void SetSDL_GLWindowFlags(const int flags) { m_SDL_WindowFlags = static_cast<SDL_WindowFlags>(m_SDL_WindowFlags | flags); }
		SDL_WindowFlags GetSDL_GLWindowFlags() const { return m_SDL_WindowFlags; }

		bool Initialize() override;
		void Deinitialize() override;

		void ClearBuffer() override;
		void SwapBuffers() override;

		std::string GetGLSLVersion() const { return m_GLSLVersion; }
		GLint GetGLMajorVersion() const { return m_GLMajorVersion; }
		GLint GetGLMinorVersion() const { return m_GLMinorVersion; }

		SDL_GLContext GetSDL_GLContext() const { return m_SDL_GLContext; }

		SDL_GLVsyncMode GetVsyncMode() const { return m_VsyncMode; }
		void SetVsyncMode(const SDL_GLVsyncMode mode) 
		{
			m_VsyncMode = mode; 
			if (SDL_GL_SetSwapInterval(static_cast<int>(m_VsyncMode)) < 0)
			{
				DE_LOG(LogWindow, Critical, "SDL_GL_SetSwapInterval failed! SDL_Error: {}", SDL_GetError())
			}
		}
		void ToggleVsync()
		{
			if (m_VsyncMode == SDL_GLVsyncMode::Off)
			{
				m_VsyncMode = SDL_GLVsyncMode::On;
			}
			else
			{
				m_VsyncMode = SDL_GLVsyncMode::Off;
			}

			if (SDL_GL_SetSwapInterval(static_cast<int>(m_VsyncMode)) < 0)
			{
				DE_LOG(LogWindow, Critical, "SDL_GL_SetSwapInterval failed! SDL_Error: {}", SDL_GetError())
			}
		}
	private:

		SDL_Window* m_SDL_GLWindow;

		SDL_WindowFlags m_SDL_WindowFlags;

		SDL_GLContext m_SDL_GLContext;

		SDL_GLVsyncMode m_VsyncMode = SDL_GLVsyncMode::Off;

		// GL Attributes
		std::string m_GLSLVersion = "#version 330";
		GLint m_GLMajorVersion = 3;
		GLint m_GLMinorVersion = 0;

		GLint m_GLDepthSize = 24;
		GLint m_GLStencilSize = 8;
		GLint m_GLDoubleBuffer = 1;

		friend class WindowSubsystem;

	};
}