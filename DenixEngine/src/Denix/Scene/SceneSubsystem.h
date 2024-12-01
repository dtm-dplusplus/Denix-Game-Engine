#pragma once

#include <typeindex>

#include "Denix/Core.h"
#include "Denix/Resource/Asset.h"
#include "Denix/Scene/Scene.h"
#include "Denix/System/SubSystem.h"
#include  "yaml-cpp/yaml.h"

namespace Denix
{

	
	/* Subsystem that manages the scenes
	* A scene must always be loaded in order to render anything
	*/ 
	class SceneSubsystem: public Subsystem
	{
	public:
		SceneSubsystem(const Ref<Asset>& _startupScene = nullptr);

		~SceneSubsystem() override
		{
			m_ActiveScene = nullptr;

			s_SceneSubsystem = nullptr;
		}

		
		void CleanRubbish();
		bool m_SceneThreaded = false;

	public:
		void Initialize() override;

		void Deinitialize() override;

		void Update(float _deltaTime) override;

		static void SerializeScene();
		static bool SerializeScene(const Scene* _scene);

		// Template function to deserialize a scene with derived scene class type.
		// Without this, derived scene data will be lost.
		// This is a tempory solution until reflection is implemented.
		template <class T = Scene>
		static Ref<Scene> DeserializeScene(const Ref<Asset>& _sceneAsset);

		static void DeserializeScene(const Ref<Scene>& _scene);

		static bool DeserializeSceneObjects(const YAML::Node& _sceneNode, std::vector<Ref<Actor>>& _actors);
		
		static void SpawnSceneObject(const Ref<Actor>& _object);
		
		static SceneSubsystem* Get() { return s_SceneSubsystem; }

		static Ref<Scene> GetActiveScene() { return s_SceneSubsystem->m_ActiveScene; }

		Ref<Camera> GetActiveCamera() const;

	public:
		static bool LoadScene(const Ref<Scene>& _scene);

		static void UnloadScene(const std::string& _name);

		// Open Scene Methods. The string & asset overloads are wrappers for the pass by scene method.
		static void OpenScene(const std::string& _name);
		static void OpenScene(const Ref<Asset>& _sceneAsset);
		static void OpenScene(const Ref<Scene>& _scene);

		
		/**
		 * Start the scene. This is called by the editor when the play button is pressed.
		 * If this is a shipped game, this function should be called when the game starts.
		 */
		static void PlayScene();

		static void StopScene();

		static void PauseScene();

	private:
		static SceneSubsystem* s_SceneSubsystem;

		Ref<Asset> m_StartupScene;
		
		std::unordered_map<std::string, Ref<Scene>> m_LoadedScenes;

		Ref<Scene> m_ActiveScene;

		friend class Engine;
		friend class EditorSubsystem;
	};

	
	template <class T>
	Ref<Scene> SceneSubsystem::DeserializeScene(const Ref<Asset>& _sceneAsset)
	{
		try
		{
			Ref<Scene> newScene = CastRef<Scene>(MakeRef<T>(_sceneAsset));
			if (!newScene)
			{
				DE_LOG(LogScene, Error, "Failed to create scene object")
				return nullptr;
			}
				
			// Load the scene data from the asset file
			YAML::Node sceneNode = YAML::LoadFile(_sceneAsset->GetAssetPath());
    
			// Check if the scene data is valid
			if (!sceneNode)
			{
				DE_LOG(LogScene, Error, "Failed to load scene asset data: {}", _sceneAsset->GetAssetPath())
				return nullptr;
			}
        		
			// Load the scene objects
			std::vector<Ref<Actor>> sceneActors;
			DeserializeSceneObjects(sceneNode, sceneActors);
			
			for (const auto& newActor : sceneActors)
				newScene->SpawnActor(newActor);
			
			DE_LOG(LogScene, Info, "Deserialized scene: {}", _sceneAsset->GetAssetName())

			return newScene;
		}
		catch (const std::exception& e)
		{
			DE_LOG(LogScene, Error, "Failed to deserialize scene: {}", e.what())
			return nullptr;
		}
	}
}