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

	Ref<WindowSubSystem> GetWindowSubSystem() const { return m_WindowSubSystem; }
	Ref<Window> GetEngineWindow() const { return m_EngineWindow; }

	Ref<SceneSubSystem> GetSceneSubSystem() const { return m_SceneSubSystem; }
	Ref<Scene> GetEngineScene() const { return m_SceneSubSystem->m_ActiveScene; }

	Ref<UISubSystem> GetUISubSystem() const { return m_UISubSytem; }

private:
	static Engine* s_Engine;

	bool m_Running;

	Ref<WindowSubSystem> m_WindowSubSystem;
	Ref<Window> m_EngineWindow;

	Ref<UISubSystem> m_UISubSytem;

	Ref<SceneSubSystem> m_SceneSubSystem;
	Ref<Scene> m_EngineScene;
};