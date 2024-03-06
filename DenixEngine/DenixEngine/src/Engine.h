#pragma once

#include "Core.h"

class WindowSubSystem;
class ShaderSubSystem;
class SceneSubSystem;
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

private:
	static Engine* s_Engine;

	//std::vector<Ref<SubSystem>>  m_SubSystems;

	Ref<WindowSubSystem> m_WindowSubSystem;

	Ref<SceneSubSystem> m_SceneSubSystem;

	Ref<ShaderSubSystem> m_ShaderSubSystem;
	Ref<UISubSystem> m_UISubSytem;
};