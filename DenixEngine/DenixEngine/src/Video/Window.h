#pragma once

#include <string>

#include "SDL_video.h"

class Window
{
public:

	Window(): m_SDL_Window{nullptr}, m_IsOpen{false}, m_Title{"Window"}, m_WinX{800}, m_WinY{600} {}

	virtual bool Start(SDL_WindowFlags _flags);
	virtual void Stop();

	virtual void ClearBuffer();
	virtual void SwapBuffers();

	virtual void PollEvents();

protected:
	SDL_Window* m_SDL_Window;

	bool m_IsOpen;

	std::string m_Title;

	// Will be floats when moving to SDL3
	float m_WinX;
	float m_WinY;

	friend class Engine;
};