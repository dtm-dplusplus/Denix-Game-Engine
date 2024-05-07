#pragma once

#include "Denix/Core.h"
#include "Denix/Scene/Scene.h"
#include "Denix/System/SubSystem.h"

namespace Denix
{
	/* Subsystem that manages the scenes
	* A scene must always be loaded in order to render anything
	*/ 
	class SceneSubsystem: public Subsystem
	{
	public:
		SceneSubsystem()
		{
			s_SceneSubsystem = this;

			DE_LOG_CREATE(LogSceneSubSystem)
			DE_LOG_CREATE(LogScene)
			DE_LOG_CREATE(LogObject)
		}

		~SceneSubsystem() override
		{
			m_ActiveScene = nullptr;

			s_SceneSubsystem = nullptr;
		}

		
		void CleanRubbish();

	public:
		void Initialize() override;

		void Deinitialize() override;

		void Update(float _deltaTime) override;

		void GameObjectsUpdate(float _deltaTime);


		static SceneSubsystem* Get() { return s_SceneSubsystem; }

		Ref<Scene> GetActiveScene() const { return m_ActiveScene; }

		Ref<Camera> GetActiveCamera() const
		{
			if (m_ActiveScene)
			{
				return m_ActiveScene->GetViewportCamera();
			}

			DE_LOG(LogSceneSubSystem, Error, "No active scene")
				return nullptr;
		}
	public:
		bool LoadScene(const Ref<Scene>& _scene);

		void UnloadScene(const std::string& _name);

		void OpenScene(const std::string& _name);

		void PlayScene();

		void StopScene();

		void PauseScene();



	private:
		static SceneSubsystem* s_SceneSubsystem;

		std::unordered_map<std::string, Ref<Scene>> m_LoadedScenes;

		Ref<Scene> m_ActiveScene;

		friend class Engine;
		friend class EditorSubsystem;
	};
}
