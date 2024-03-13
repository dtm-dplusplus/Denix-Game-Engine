#pragma once

#include "Denix/Core.h"
#include "Denix/Scene/Scene.h"
#include "SubSystem.h"

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
			DE_LOG_CREATE(LogObject)
		}

		~SceneSubSystem() override
		{
			m_ActiveScene = nullptr;
			s_SceneSubSystem = nullptr;
		}

		void CleanRubbish();

	public:
		void Initialize() override;

		void Deinitialize() override;

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
		bool LoadScene(const Ref<Scene>& _scene);

		void UnloadScene(const std::string& _name);

		void OpenScene(const std::string& _name);

		void PlayScene();

		void StopScene();

		void PauseScene();

	private:
		static SceneSubSystem* s_SceneSubSystem;

		std::unordered_map<std::string, Ref<Scene>> m_LoadedScenes;

		Ref<Scene> m_ActiveScene;
		
		std::vector< Ref<class TransformComponent>> m_TransformComponets;

		class PhysicsSubSystem* s_PhysicsSubSystem;
		class WindowSubSystem* s_WindowSubSystem;

	private:
		// TEMP ImGui
		int ObjectSelection = 0;
		bool ScenePanelOpen = true;
		bool ShowDemoWindow = false;

		float DragSpeed = 1.0f;
		float DragSpeedDelta;

		void ScenePanel();
		void DetailsPanel();

		friend class Engine;
	};
}
