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
			s_RendererSubSystem = nullptr;
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

		void RegisterTransformComponent(const Ref<TransformComponent>& _transformComponent)
		{
			DE_LOG(LogSceneSubSystem, Trace, "TransformComponent Registered: #{} {}", _transformComponent->GetID(), _transformComponent->GetName())
			m_TransformComponets.push_back(_transformComponent);
		}

		void UnregisterTransformComponent(const Ref<TransformComponent>& _transformComponent)
		{
			DE_LOG(LogSceneSubSystem, Trace, "TransformComponent Unregistered: #{} {}", _transformComponent->GetID(), _transformComponent->GetName())

			std::erase(m_TransformComponets, _transformComponent);
		}	

		void UnregisterTransformComponent(const int _transformComponentID)
		{
			for (const auto& transformComponent : m_TransformComponets)
			{
				if (transformComponent->GetID() == _transformComponentID)
				{
					DE_LOG(LogSceneSubSystem, Trace, "TransformComponent Unregistered: #{} {}", transformComponent->GetID(), transformComponent->GetName())
					std::erase(m_TransformComponets, transformComponent);
				}
			}
		}

	private:
		static SceneSubSystem* s_SceneSubSystem;

		std::unordered_map<std::string, Ref<Scene>> m_LoadedScenes;

		Ref<Scene> m_ActiveScene;
		
		std::vector< Ref<class TransformComponent>> m_TransformComponets;

		class PhysicsSubSystem* s_PhysicsSubSystem;
		class WindowSubSystem* s_WindowSubSystem;
		class RendererSubSystem* s_RendererSubSystem;
	private:
	

		friend class Engine;
	};
}
