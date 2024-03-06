#pragma once

#include "Core.h"
#include "SubSystem.h"
#include "Video/Window.h"
#include <SDL.h>
#include <GL/glew.h>

class WindowSubSystem: public SubSystem
{
public:
    WindowSubSystem()
    {
		s_WindowSubSystem = this;
    }

    ~WindowSubSystem() override
    {
		s_WindowSubSystem = nullptr;
    }

	static WindowSubSystem* Get() { return s_WindowSubSystem; }

    void Initialize() override
    {
		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0)
		{
			DE_LOG(Log, Critical, "SDL Init failed! SDL_Error: {}", SDL_GetError())
				return;
		}
		DE_LOG(Log, Trace, "SDL Init success")

		// Set SDL OpenGL Version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_GLMajorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_GLMinorVersion);

		// Useful for virtual keyboards, complex chararacter sets
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

		// Set GL Attributes
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, m_GLDoubleBuffer);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_GLDepthSize);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, m_GLStencilSize);

		//Create window
		SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
		m_Window = std::make_shared<Window>();
		if (!m_Window->Start(flags))
			return;

		// Init Glew
		if (glewInit() != GLEW_OK)
		{
			DE_LOG(Log, Critical, "glewInit failed!")
				return;
		}
		DE_LOG(Log, Trace, "glewInit success")

		// Enable Blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        DE_LOG(LogWindow, Trace, "Window Subsystem Initialized")

        m_Initialized = true;
    }

    void Deinitialize() override
    {
		m_Window->Stop();

		SDL_Quit();

        DE_LOG(LogWindow, Info, "Window Subsystem Deinitialized")

        m_Initialized = false;
    }   

	Ref<Window> GetWindow() const { return m_Window; }

	std::string GetGLSLVersion() const { return m_GLSLVersion; }
	GLint GetGLMajorVersion() const { return m_GLMajorVersion; }
	GLint GetGLMinorVersion() const { return m_GLMinorVersion; }


private:
    Ref<Window> m_Window;

	// GL Attributes
	std::string m_GLSLVersion = "#version 130";
	GLint m_GLMajorVersion = 3;
	GLint m_GLMinorVersion = 0;

	GLint m_GLDepthSize = 24;
	GLint m_GLStencilSize = 8;
	GLint m_GLDoubleBuffer = 1;


	static WindowSubSystem* s_WindowSubSystem;

	friend class Engine;
};

