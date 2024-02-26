#pragma once

#include "Core.h"


class Engine
{
public:

	Engine();
	~Engine();

	void Initialize();
	void Deinitialize();

	void Run();

private:

	bool m_Running;

	std::shared_ptr<class Window> m_Window;

	std::vector<std::weak_ptr<class SubSystem>> m_SubSystems;
	
	std::shared_ptr<class SceneSubSystem> m_SceneSubSystem;
};