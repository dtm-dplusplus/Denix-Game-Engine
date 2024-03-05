#pragma once

#include "Core.h"
#include "Scene/Scene.h"
#include "Scene/DefaultScene.h"
#include "SubSystem.h"

/* Subsystem that manages the scenes
* A scene must always be loaded in order to render anything
*/ 
class SceneSubSystem: public SubSystem
{
public:
	SceneSubSystem()
	{
	}

	~SceneSubSystem() override
	{
	}

	void Initialize() override
	{
		LoadScene(std::make_shared<DefaultScene>());

		DE_LOG(LogScene, Info, "Scene Subsystem Initialized")

		m_Initialized = true;
	}

	void Deinitialize() override
	{
		m_LoadedScenes.clear();
		m_ActiveScene = nullptr;

		DE_LOG(LogScene, Info, "Scene Subsystem Deinitialized")

		m_Initialized = false;
	}

	void Update(float _deltaTime) override;

	bool LoadScene(Ref<Scene> _scene, const bool _isActiveScene = true)
	{
		// Check if the pointer is valid
		if (!_scene)
		{
			DE_LOG(LogScene, Critical, "Failed to load scene")
			return false;
		}

		// Load the scene
		if (!_scene->Load())
		{
			DE_LOG(LogScene, Critical, "Failed to load scene")
			return false;
		}

		DE_LOG(LogScene, Info, "Scene loaded: ", _scene->GetName())

		// Set the scene as active scene
		if(_isActiveScene)
		{
			m_ActiveScene = _scene;

			DE_LOG(LogScene, Info, "Scene set as active: ", _scene->GetName())
		}

		// Transfer ownership of the scene to the scene subsystem
		m_LoadedScenes.push_back(std::move(_scene));

		return true;
	}


private:

	std::vector<Ref<Scene>> m_LoadedScenes;
	Ref<Scene> m_ActiveScene;

	friend class Engine;
};