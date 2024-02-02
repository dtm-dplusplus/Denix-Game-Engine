#pragma once

class Engine
{
public:

	Engine();
	~Engine();

	bool Start();
	void Stop();

	void Run();

private:
	bool m_IsRunning;

	class SDL_Window* m_Window;

	int m_WinX;
	int m_WinY;
};
