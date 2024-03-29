#pragma once

#include "Object.h"
#include "GameObject.h"
#include "Component.h"
#include "Camera.h"
#include "Denix/System/PhysicsSubsystem.h"

namespace Denix
{
	// Basic Scene class so I can start creating and testing features
	class Scene : public Object
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
			m_SceneObjects.push_back(m_Camera);

			return true;
		}

		virtual void Unload()
		{
		}

		void BeginScene() override
		{
			

			for (const auto& obj : m_SceneObjects)
			{
				obj->BeginScene();
			}
		}


		void EndScene() override
		{
			for (const auto& obj : m_SceneObjects)
			{
				obj->EndScene();
			}
		}

		void BeginPlay() override
		{
			m_IsPlaying = true;

			for (const auto& obj : m_SceneObjects)
			{
				obj->BeginPlay();
			}
		}

		void EndPlay() override
		{
			m_IsPlaying = false;

			for (const auto& obj : m_SceneObjects)
			{
				obj->EndPlay();
			}
		}
		void Update(float _deltaTime) override {}

		bool IsPlaying() const { return m_IsPlaying; }

		glm::vec3 GetGravity() const { return m_SceneGravity; }

		Ref<Camera> GetCamera() { return m_Camera; }
	protected:
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
