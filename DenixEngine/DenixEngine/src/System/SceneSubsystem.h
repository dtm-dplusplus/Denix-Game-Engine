#pragma once

#include "Core.h"
#include "Scene/Scene.h"
#include "Scene/ExampleScenes.h"
#include "SubSystem.h"
#include "WindowSubSystem.h"
#include "../../contrib/imgui/imgui.h"

namespace Denix
{
	/* Subsystem that manages the scenes
	* A scene must always be loaded in order to render anything
	*/ 
	class SceneSubSystem: public SubSystem
	{
	public:
		SceneSubSystem()
		{
			s_SceneSubSystem = this;

			DE_LOG_CREATE(LogSceneSubSystem)
			DE_LOG_CREATE(LogScene)
			DE_LOG_CREATE(LogPhysics)
			DE_LOG_CREATE(LogObject)
		}

		~SceneSubSystem() override
		{
			m_ActiveScene = nullptr;
			s_SceneSubSystem = nullptr;
		}

		void Initialize() override
		{
			LoadScene(std::make_shared<DefaultScene>());
			OpenScene("DefaultScene");
			DE_LOG(LogSceneSubSystem, Trace, "Scene Subsystem Initialized")

			m_Initialized = true;
		}

		void Deinitialize() override
		{
			m_LoadedScenes.clear();

			DE_LOG(LogSceneSubSystem, Trace, "Scene Subsystem Deinitialized")

			m_Initialized = false;
		}

		void Update(float _deltaTime) override;

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
	public:
		bool LoadScene(const Ref<Scene>& _scene)
		{
			// Check if the pointer is valid
			if (!_scene)
			{
				DE_LOG(LogSceneSubSystem, Error, "Load Scene: Invalid scene reference")
				return false;
			}

			// Check it isn't already loaded
			if (m_LoadedScenes.contains(_scene->GetName()))
			{
				DE_LOG(LogSceneSubSystem, Error, "Load Scene: A scene name {} is already loaded", _scene->GetName())
				return false;
			}

			// Load the scene
			if (!_scene->Load())
			{
				DE_LOG(LogSceneSubSystem, Critical, "Failed to load scene")
				return false;
			}

			m_LoadedScenes[_scene->GetName()] = _scene;
			DE_LOG(LogSceneSubSystem, Trace, "Scene loaded: ", _scene->GetName())

			return true;
		}

		void UnloadScene(const std::string& _name)
		{
			if (const Ref<Scene>scene = m_LoadedScenes[_name])
			{
				scene->Unload();
				m_LoadedScenes.erase(_name);
				m_ActiveScene = nullptr;

				DE_LOG(LogSceneSubSystem, Info, "Unloaded Scene: {}", _name)
					return;
			}

			DE_LOG(LogSceneSubSystem, Error, "Load Scene: Invalid scene name, or the scene isn't loaded")


		}

		void OpenScene(const std::string& _name)
		{
			if (const Ref<Scene>scene = m_LoadedScenes[_name])
			{
				m_ActiveScene = scene;
				DE_LOG(LogSceneSubSystem, Info, "Activated Scene: {}", _name)
				return;
			}
	
			DE_LOG(LogSceneSubSystem, Error, "Cound't find Scene: {}", _name)
		}

		void PlayScene()
		{
			if (m_ActiveScene)
			{
				m_ActiveScene->BeginScene();
			
				DE_LOG(LogSceneSubSystem, Trace, "Scene Started")
			}
		}

		void StopScene()
		{
			if (m_ActiveScene)
			{
				m_ActiveScene->EndScene();

				UnloadScene(m_ActiveScene->GetName());
			
				// TEMP : this may cause memory leaks. Will move to a better solution later
				DE_LOG(LogSceneSubSystem, Trace, "Scene Stopped")

				LoadScene(std::make_shared<DefaultScene>());
				OpenScene("DefaultScene");
			}
		}

		void PauseScene()
		{
			DE_LOG(LogSceneSubSystem, Trace, "Scene Paused")
		}


	private:
		static SceneSubSystem* s_SceneSubSystem;

		std::unordered_map<std::string, Ref<Scene>> m_LoadedScenes;

		Ref<Scene> m_ActiveScene;

		friend class Engine;
	};
}
