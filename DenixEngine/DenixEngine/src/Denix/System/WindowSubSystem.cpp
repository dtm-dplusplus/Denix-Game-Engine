#include "DePch.h"
#include "WindowSubSystem.h"
#include <SDL.h>
#include <GL/glew.h>

#include "Denix/Core/DeLog.h"
Denix::WindowSubSystem* Denix::WindowSubSystem::s_WindowSubSystem{nullptr};

void Denix::WindowSubSystem::Initialize()
{
	//Create window
	if(const Ref<SDL_GLWindow> window = MakeRef<SDL_GLWindow>())
	{
		if(!window->Initialize()) return;
		m_Window = window;
	}
		
	// Init Glew
	if (glewInit() != GLEW_OK)
	{
		DE_LOG(Log, Critical, "glewInit(): failed")
		return;
	}
	DE_LOG(Log, Trace, "glewInit(): success")

	DE_LOG(LogWindow, Trace, "Window Subsystem Initialized")

	m_Initialized = true;
}

void Denix::WindowSubSystem::Deinitialize()
{
	m_Window->Deinitialize();

	SDL_Quit();

	DE_LOG(LogWindow, Trace, "Window Subsystem Deinitialized")

	m_Initialized = false;
}
