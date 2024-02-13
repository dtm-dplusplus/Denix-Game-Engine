#pragma once

#include <memory>

class Engine
{
public:

	Engine();

	bool Start();
	void Stop();

	void Run();

private:

	std::shared_ptr<class Window> m_Window;
};