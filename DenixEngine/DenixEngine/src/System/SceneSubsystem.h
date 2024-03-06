#pragma once

#include "Core.h"
#include "Scene/Scene.h"
#include "Scene/DefaultScene.h"
#include "SubSystem.h"
#include "WindowSubSystem.h"
#include "../../contrib/imgui/imgui.h"

/* Subsystem that manages the scenes
* A scene must always be loaded in order to render anything
*/ 
class SceneSubSystem: public SubSystem
{
public:
	SceneSubSystem()
	{
		s_SceneSubSystem = this;

		DE_LOG_CREATE(LogSceneSubSystemSubSystem)
		DE_LOG_CREATE(LogSceneSubSystem)
		DE_LOG_CREATE(LogPhysics)
		DE_LOG_CREATE(LogObject)
	}

	~SceneSubSystem() override
	{
		s_SceneSubSystem = nullptr;
	}

	void Initialize() override
	{
		LoadScene(std::make_shared<DefaultScene>());

		DE_LOG(LogSceneSubSystemSubSystem, Trace, "Scene Subsystem Initialized")

		m_Initialized = true;
	}

	void Deinitialize() override
	{
		m_ActiveScene = nullptr;

		DE_LOG(LogSceneSubSystemSubSystem, Trace, "Scene Subsystem Deinitialized")

		m_Initialized = false;
	}

	void Update(float _deltaTime) override;


	bool LoadScene(const Ref<Scene>& _scene, const bool _isActiveScene = true)
	{
		// Check if the pointer is valid
		if (!_scene)
		{
			DE_LOG(LogSceneSubSystem, Critical, "Failed to load scene")
			return false;
		}

		// Load the scene
		if (!_scene->Load())
		{
			DE_LOG(LogSceneSubSystem, Critical, "Failed to load scene")
			return false;
		}

		DE_LOG(LogSceneSubSystem, Trace, "Scene loaded: ", _scene->GetName())

		// Set the scene as active scene
		if(_isActiveScene)
		{
			m_ActiveScene = _scene;

			DE_LOG(LogSceneSubSystem, Trace, "Scene set as active: ", _scene->GetName())
		}

		_scene->m_Camera = std::make_shared<Camera>();

		m_ActiveScene = _scene;

		return true;
	}

	void PlayScene()
	{
		if (m_ActiveScene)
		{
			for (const auto& obj : m_ActiveScene->m_SceneObjects)
			{
				obj->BeginScene();
			}

			m_ActiveScene->BeginScene();
			
			m_ActiveScene->m_IsLive = true;

			DE_LOG(LogSceneSubSystemSubSystem, Trace, "Scene Started")
		}
	}

	void PauseScene()
	{
		DE_LOG(LogSceneSubSystem, Trace, "Scene Paused")
	}

	void StopScene()
	{
		if (m_ActiveScene)
		{
			for (const auto& obj : m_ActiveScene->m_SceneObjects)
			{
				obj->EndScene();
			}

			m_ActiveScene->EndScene();
			
			m_ActiveScene->m_IsLive = false;
			
			DE_LOG(LogSceneSubSystem, Trace, "Scene Stopped")

			LoadScene(std::make_shared<DefaultScene>());
		}
	}

public:
	static SceneSubSystem* Get() { return s_SceneSubSystem; }

	Ref<Scene> GetActiveScene() const { return m_ActiveScene; }

	Ref<Camera> GetActiveCamera() const
	{
		if (m_ActiveScene)
		{
			return m_ActiveScene->GetCamera();
		}

		DE_LOG(LogSceneSubSystem, Error, "No active scene")
		return nullptr;
	}
private:
	static SceneSubSystem* s_SceneSubSystem;
	Ref<Scene> m_ActiveScene;


	friend class Engine;
};