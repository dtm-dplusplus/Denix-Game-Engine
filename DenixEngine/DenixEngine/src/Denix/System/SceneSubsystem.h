#pragma once

#include "Denix/Core.h"
#include "Denix/Scene/Scene.h"
#include "Subsystem.h"

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
			s_RendererSubsystem = nullptr;
			s_PhysicsSubsystem = nullptr;

			s_SceneSubsystem = nullptr;
		}

		enum class ViewportMode
		{
			Render,
			Collider
		};
		void CleanRubbish();

	public:
		void Initialize() override;

		void Deinitialize() override;

		void Update(float _deltaTime) override;

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

		static void RegisterTransformComponent(const Ref<TransformComponent>& _transformComponent)
		{
			DE_LOG(LogSceneSubSystem, Trace, "TransformComponent Registered: #{} {}", _transformComponent->GetID(), _transformComponent->GetName())
			s_SceneSubsystem->m_TransformComponents.push_back(_transformComponent);
		}

		static void UnregisterTransformComponent(const Ref<TransformComponent>& _transformComponent)
		{
			DE_LOG(LogSceneSubSystem, Trace, "TransformComponent Unregistered: #{} {}", _transformComponent->GetID(), _transformComponent->GetName())

			std::erase(s_SceneSubsystem->m_TransformComponents, _transformComponent);
		}	

		static void UnregisterTransformComponent(const int _transformComponentID)
		{
			for (const auto& transformComponent : s_SceneSubsystem->m_TransformComponents)
			{
				if (transformComponent->GetID() == _transformComponentID)
				{
					DE_LOG(LogSceneSubSystem, Trace, "TransformComponent Unregistered: #{} {}", transformComponent->GetID(), transformComponent->GetName())
					std::erase(s_SceneSubsystem->m_TransformComponents, transformComponent);
				}
			}
		}

		static void SetViewportMode(int _mode)
		{
			if(_mode >= 0 && _mode <= static_cast<int>(ViewportMode::Collider))
				s_SceneSubsystem->m_ViewportMode = _mode;
		}
		static int& GetViewportMode() { return s_SceneSubsystem->m_ViewportMode; }

	private:
		static SceneSubsystem* s_SceneSubsystem;

		class InputSubsystem* s_InputSubsystem;
		class WindowSubsystem* s_WindowSubsystem;
		class RendererSubsystem* s_RendererSubsystem;
		class PhysicsSubsystem* s_PhysicsSubsystem;
		std::unordered_map<std::string, Ref<Scene>> m_LoadedScenes;

		Ref<Scene> m_ActiveScene;
		
		std::vector< Ref<class TransformComponent>> m_TransformComponents;
		
		int m_ViewportMode = static_cast<int>(ViewportMode::Render);

		friend class Engine;
		friend class EditorSubsystem;
	};
}
