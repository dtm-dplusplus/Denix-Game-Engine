#pragma once

#include "Object.h"
#include "GameObject.h"
#include "Component.h"
#include "Camera.h"

namespace Denix
{
	// Basic Scene class so I can start creating and testing features
	class Scene: public Object
	{
	public:

		Scene() = default;

		Scene(const ObjectInitializer& _object_init) : Object(_object_init), m_Camera{ MakeRef<Camera>() }
		{
		}

		Scene(const Scene& _other) : Object(_other), m_IsLive(_other.m_IsLive), m_SceneGravity(_other.m_SceneGravity)
		{
			// Deep copy the m_SceneObjects vector
			for (const auto& gameObject : _other.m_SceneObjects)
			{
				m_SceneObjects.push_back(MakeRef<GameObject>(*gameObject));
			}

			// Deep copy the m_Camera object
			if (_other.m_Camera)
			{
				m_Camera = MakeRef<Camera>(*_other.m_Camera);
			}
		}

		~Scene() override = default;

		virtual bool Load()
		{
			m_Camera = MakeRef<Camera>();

			if (!m_Camera)
			{
				return false;
			}

			return true;
		}

		virtual void Unload() 
		{
		}

		void BeginScene() override 
		{
			m_IsLive = true;

			for (const auto& obj : m_SceneObjects)
			{
				obj->BeginScene();
			}
		}

		void EndScene() override 
		{
			m_IsLive = false;

			for (const auto& obj : m_SceneObjects)
			{
				obj->EndScene();
			}
		}

		void Update(float _deltaTime) override{}

		bool IsLive() const { return m_IsLive; }

		glm::vec3 GetGravity() const { return m_SceneGravity; }

		Ref<Camera> GetCamera() { return m_Camera; }
	protected:
		/** determine if the engine is in editor or tool side mode.
		 * True if the scene is being played. False if in editor mode.
		 */
		bool m_IsLive = false;

		glm::vec3 m_SceneGravity = glm::vec3(0.0f, -9.8f, 0.0f);

		/** List of Objects in the scene */
		std::vector<Ref<GameObject>> m_SceneObjects;

		Ref<Camera> m_Camera;

		friend class SceneSubSystem;
	};
}
