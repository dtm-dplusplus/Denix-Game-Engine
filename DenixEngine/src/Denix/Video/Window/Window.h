#pragma once

#include <gl/glew.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

#include "Denix/Core.h"
#include "Denix/Core/Math/Math.h"

namespace Denix
{
	class Window
	{
	public:
		Window() : m_IsOpen{ false }, m_Title{ "Denix Engine" }, m_WinX{ 1920 }, m_WinY{ 1080 } {}

		virtual ~Window() = default;

		virtual void ClearBuffer() = 0;
		virtual void SwapBuffers() = 0;

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
		friend class InputSubsystem;
	};


	class SDL_GLWindow final : public Window
	{
	public:
		SDL_GLWindow();

		~SDL_GLWindow() override;

		void ToggleFullscreen()
		{
			if (m_IsFullscreen)
			{
				SDL_SetWindowFullscreen(m_SDL_GLWindow, SDL_WINDOW_MAXIMIZED);
				m_IsFullscreen = false;
			}
			else
			{
				SDL_SetWindowFullscreen(m_SDL_GLWindow, SDL_WINDOW_FULLSCREEN);
				m_IsFullscreen = true;
			}
			DE_LOG(LogWindow, Info, "Toggled Fullscreen: {}", m_IsFullscreen)
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

		// void SetSDL_GLWindowFlags(const int flags) { m_SDL_WindowFlags = m_SDL_WindowFlags | static_cast<SDL_WindowFlags>(flags); }
		SDL_WindowFlags GetSDL_GLWindowFlags() const { return m_SDL_WindowFlags; }

		void ClearBuffer() override;
		void SwapBuffers() override;

		static std::string GetGLSLVersion() { return m_GLSLVersion; }
		static int GetGLMajorVersion() { return m_GLMajorVersion; }
		static int GetGLMinorVersion() { return m_GLMinorVersion; }

		SDL_GLContext GetSDL_GLContext() const { return m_SDL_GLContext; }

		SDL_GLVsyncMode GetVsyncMode() const { return m_VsyncMode; }
		void SetVsyncMode(const SDL_GLVsyncMode mode) 
		{
			m_VsyncMode = mode; 
			if (!SDL_GL_SetSwapInterval(static_cast<int>(m_VsyncMode)))
			{
				DE_LOG(LogWindow, Error, "SDL_GL_SetSwapInterval failed! SDL_Error: {}", SDL_GetError())
			}
		}
		
	private:
		void ToggleVsync()
		{
			m_VsyncMode = static_cast<SDL_GLVsyncMode>(!static_cast<bool>(m_VsyncMode));
			SetVsyncMode(m_VsyncMode);
		}
		
		SDL_Window* m_SDL_GLWindow;

		SDL_WindowFlags m_SDL_WindowFlags;

		SDL_GLContext m_SDL_GLContext;

		SDL_GLVsyncMode m_VsyncMode = SDL_GLVsyncMode::Off;

		// GL Attributes
		inline static std::string m_GLSLVersion = "#version 330";
		inline static int m_GLMajorVersion = 3;
		inline static int m_GLMinorVersion = 0;

		inline static int m_GLDepthSize = 24;
		inline static int m_GLStencilSize = 8;
		inline static int m_GLDoubleBuffer = 1;

		friend class WindowSubsystem;
		friend class InputSubsystem;
	};
}