#pragma once

#include "Object.h"
#include "GameObject.h"
#include "Component.h"
#include "Camera.h"
#include "System/PhysicsSubSystem.h"

namespace Denix
{
	// Basic Scene class so I can start creating and testing features
	class Scene: public Object
	{
	public:

		Scene() = default;

		Scene(const ObjectInitializer& _object_init) : Object(_object_init), m_Camera{ nullptr }
		{
		}


		~Scene() override = default;

		virtual bool Load()
		{
			m_Camera = MakeRef<Camera>();


			return true;
		}

		virtual void Unload() 
		{
			m_Camera = nullptr;
		}

		void BeginScene() override 
		{
			m_IsLive = true;

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
		friend class Engine;
	};
}
