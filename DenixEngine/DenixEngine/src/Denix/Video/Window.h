#pragma once

#include "Denix/Core.h"

#include "SDL_video.h"
#include "glm/glm.hpp"
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

		virtual void PollEvents() = 0;

		bool IsOpen() const { return m_IsOpen; }

		glm::vec2 GetWindowSize() const { return { m_WinX, m_WinY }; }

		int GetWindowSizeX() const { return m_WinX; }
		int GetWindowSizeY() const { return m_WinY; }

		std::string GetTitle() const { return m_Title; }


	protected:
		bool m_IsOpen;

		std::string m_Title;

		int m_WinX;
		int m_WinY;

		friend class WindowSubSystem;
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

		void PollEvents() override;

		std::string GetGLSLVersion() const { return m_GLSLVersion; }
		GLint GetGLMajorVersion() const { return m_GLMajorVersion; }
		GLint GetGLMinorVersion() const { return m_GLMinorVersion; }

		SDL_GLContext GetSDL_GLContext() const { return m_SDL_GLContext; }

		SDL_GLVsyncMode GetVsyncMode() const { return m_VsyncMode; }
		void SetVsyncMode(const SDL_GLVsyncMode mode) { m_VsyncMode = mode; }

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

		friend class WindowSubSystem;

	};
}