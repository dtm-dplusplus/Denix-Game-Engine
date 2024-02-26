#pragma once

#include "Core.h"

#include "System/SceneSubsystem.h"
#include "System/WindowSubsystem.h"

class UISubSystem;

class Engine
{
public:

	Engine();
	~Engine();

	void Initialize();
	void Deinitialize();

	void Run();

	static Engine& Get() { return *s_Engine; }

	std::shared_ptr<WindowSubSystem> GetWindowSubSystem() const { return m_WindowSubSystem; }
	std::shared_ptr<Window> GetEngineWindow() const { return m_EngineWindow; }

	std::shared_ptr<SceneSubSystem> GetSceneSubSystem() const { return m_SceneSubSystem; }
	std::shared_ptr<Scene> GetEngineScene() const { return m_SceneSubSystem->m_ActiveScene; }

	std::shared_ptr<UISubSystem> GetUISubSystem() const { return m_UISubSytem; }

private:
	static Engine* s_Engine;

	bool m_Running;

	std::shared_ptr<WindowSubSystem> m_WindowSubSystem;
	std::shared_ptr<Window> m_EngineWindow;

	std::shared_ptr<UISubSystem> m_UISubSytem;

	std::shared_ptr<SceneSubSystem> m_SceneSubSystem;
	std::shared_ptr<Scene> m_EngineScene;
};