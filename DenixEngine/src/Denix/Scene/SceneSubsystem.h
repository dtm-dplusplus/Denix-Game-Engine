#pragma once

#include "Denix/Core.h"
#include "Denix/Scene/Scene.h"
#include "Denix/System/SubSystem.h"

namespace YAML
{
	class Node;
}

namespace Denix
{
	class Assset;
	
	/* Subsystem that manages the scenes
	* A scene must always be loaded in order to render anything
	*/ 
	class SceneSubsystem final : public Subsystem<SceneSubsystem>
	{
	public:
		explicit SceneSubsystem(const Ref<Asset>& _startupScene = nullptr);

		~SceneSubsystem() override = default;

		SceneSubsystem(const SceneSubsystem& _other) = delete;
		SceneSubsystem(SceneSubsystem&& _other) noexcept = delete;
		SceneSubsystem& operator=(const SceneSubsystem& _other) = delete;
		SceneSubsystem& operator=(SceneSubsystem&& _other) noexcept = delete;
		

		bool m_BatchUpdateActors;

		static Ref<Scene> GetActiveScene() { return s_Instance->m_ActiveScene; }

		static Ref<Camera> GetActiveCamera();

		static bool LoadScene(const Ref<Scene>& _scene);

		static void UnloadScene(const std::string& _name);

		// Open Scene Methods. The string & asset overloads are wrappers for the pass by scene method.
		static void OpenScene(const Ref<Asset>& _sceneAsset);
		static void OpenScene(const Ref<Scene>& _scene);
		
	private:
		/**
		 * Start the scene. This is called by the editor when the play button is pressed.
		 * If this is a shipped game, this function should be called when the game starts.
		 */
		static void PlayScene();

		static void StopScene();

		static void PauseScene();


		static void SerializeScene();
		static bool SerializeScene(const Ref<Scene>& _scene);

		static void DeserializeScene(const Ref<Scene>& _scene);

		static bool DeserializeSceneObjects(const Ref<Scene>& _scene, const YAML::Node& _sceneNode);

		void CleanRubbish();

		void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

		void Initialize() override;

		void Deinitialize() override;
		
		Ref<Asset> m_StartupScene;
		
		std::unordered_map<std::string, Ref<Scene>> m_LoadedScenes;

		Ref<Scene> m_ActiveScene;

		friend class Engine;
		friend class EditorSubsystem;
	};
}