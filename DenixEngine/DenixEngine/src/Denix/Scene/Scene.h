#pragma once

#include "GameObject.h"
#include "Camera.h"

namespace Denix
{
	// Basic Scene class
	class Scene
	{
	public:

		Scene() = default;

		Scene(const std::string& _name) :m_Camera{ nullptr }
		{
		}

		virtual ~Scene() = default;

		std::string GetName() const { return m_SceneName; }

		virtual bool Load()
		{
			m_Camera = MakeRef<Camera>();
			m_SceneObjects.push_back(m_Camera);

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

		Ref<Camera> GetCamera() { return m_Camera; }
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

		Ref<Camera> m_Camera;

		friend class SceneSubsystem;
		friend class EditorSubsystem;
		friend class Engine;
	};
}
