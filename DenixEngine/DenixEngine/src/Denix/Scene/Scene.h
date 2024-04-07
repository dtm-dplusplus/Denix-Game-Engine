#pragma once

#include "GameObject.h"
#include "Object/Light/LightObject.h"
#include "Camera.h"

namespace Denix
{
	constexpr unsigned int MAX_POINT_LIGHTS = 3;

	// Basic Scene class
	class Scene
	{
	public:

		Scene() = default;

		Scene(const std::string& _name) :m_ViewportCamera{ nullptr }
		{
		}

		virtual ~Scene() = default;

		std::string GetSceneName() const { return m_SceneName; }

		virtual bool Load()
		{
			m_ViewportCamera = MakeRef<Camera>();
			m_SceneObjects.push_back(m_ViewportCamera);

			m_DirLight = MakeRef<DirectionalLight>();
			m_DirLight->GetTransformComponent()->SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));
			m_SceneObjects.push_back(m_DirLight);

			return true;
		}

		virtual void Unload()
		{
		}

		virtual void BeginScene()
		{
			for (const auto& obj : m_SceneObjects)
			{
				obj->BeginScene();
			}
		}


		virtual void EndScene()
		{
			for (const auto& obj : m_SceneObjects)
			{
				obj->EndScene();
			}
		}

		virtual void BeginPlay()
		{
			m_IsPlaying = true;

			for (const auto& obj : m_SceneObjects)
			{
				obj->BeginPlay();
			}
		}

		virtual void EndPlay()
		{
			m_IsPlaying = false;

			for (const auto& obj : m_SceneObjects)
			{
				obj->EndPlay();
			}
		}
		virtual void Update(float _deltaTime) {}

		bool IsPlaying() const { return m_IsPlaying; }

		glm::vec3 GetGravity() const { return m_SceneGravity; }

		Ref<Camera> GetViewportCamera() { return m_ViewportCamera; }
	protected:

		/** Name of the scene. Must be uniqiue */
		std::string m_SceneName;

		/** determine if the engine is in editor or tool side mode.
		 * True if the scene is being played. False if in editor mode.
		 */
		bool m_IsPlaying = false;

		/** Gravity of the scene */
		glm::vec3 m_SceneGravity = glm::vec3(0.0f, -9.81f, 0.0f);

		/** List of Objects in the scene */
		std::vector<Ref<GameObject>> m_SceneObjects;

		Ref<Camera> m_ViewportCamera;

		Ref<DirectionalLight> m_DirLight;

		friend class SceneSubsystem;
		friend class RendererSubsystem;
		friend class EditorSubsystem;
		friend class Engine;
	};
}
