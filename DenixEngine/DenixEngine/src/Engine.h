#pragma once

class Engine
{
public:

	Engine();

	bool Start();
	void Stop();

	void Run();

private:
	bool m_IsRunning;

	struct SDL_Window* m_Window;

	int m_WinX;
	int m_WinY;
};