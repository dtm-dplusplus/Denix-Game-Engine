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
		LoadActiveScene(std::make_shared<DefaultScene>());

		DE_LOG(LogScene, Info, "Scene Subsystem Initialized")

		m_Initialized = true;
	}

	bool LoadScene(std::shared_ptr<Scene> _scene)
	{
		if (!_scene)
		{
			DE_LOG(LogScene, Critical, "Failed to load scene")
				return false;
		}


		if (!_scene->Load())
		{
			DE_LOG(LogScene, Critical, "Failed to load scene")
				return false;
		}

		DE_LOG(LogScene, Info, "Scene loaded: ", _scene->GetName())

		// Transfer ownership of the scene to the scene subsystem
		m_LoadedScenes.push_back(std::move(_scene));

			return true;
	}

	bool LoadActiveScene(std::shared_ptr<Scene> _scene)
	{
		LoadScene(_scene);
		m_ActiveScene = m_LoadedScenes.back();
		return true;
	}
private:

	std::vector<std::shared_ptr<Scene>> m_LoadedScenes;
	std::shared_ptr<Scene> m_ActiveScene;

	friend class Engine;
};

