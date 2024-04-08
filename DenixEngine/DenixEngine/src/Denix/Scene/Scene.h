#pragma once

#include "GameObject.h"
#include "Object/Light/LightObject.h"
#include "Camera.h"

namespace Denix
{
	constexpr unsigned int MAX_POINT_LIGHTS = 3;
	constexpr unsigned int MAX_SPOT_LIGHTS = 3;

	// Basic Scene class
	class Scene
	{
	public:

		Scene(const std::string& _name) :
			m_SceneName{ _name },
			m_ViewportCamera{ nullptr },
			m_ActiveCamera{ nullptr },
			m_DirLight{ nullptr }
		{
		}

		virtual ~Scene() = default;

		std::string GetSceneName() const { return m_SceneName; }

		virtual bool Load()
		{
			m_ViewportCamera = MakeRef<ViewportCamera>();
			m_ViewportCamera->GetTransformComponent()->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
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
			m_ActiveCamera = m_ViewportCamera;

			for (const auto& obj : m_SceneObjects)
			{
				obj->BeginScene();

				// We need to count the number of point lights in the scene
				// Currently this is done statically at the start. Meaning there is no way to add or remove point lights at runtime
				if (typeid(PointLight) == typeid(*obj))
				{
					DE_LOG(LogScene, Info, "Point Light Found")
					m_PointLights.push_back(std::static_pointer_cast<PointLight>(obj));
				}

				if (typeid(SpotLight) == typeid(*obj))
				{
					DE_LOG(LogScene, Info, "Spot Light Found")
					m_SpotLights.push_back(std::static_pointer_cast<SpotLight>(obj));
				}
			}
		}


		virtual void EndScene()
		{
			for (const auto& obj : m_SceneObjects)
			{
				obj->EndScene();
			}

			// Give camera back to viewport camera
			m_ActiveCamera = m_ViewportCamera;
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

		Ref<Camera> GetActiveCamera() { return m_ActiveCamera; }

		Ref<Camera> GetGameCamera() const
		{
			for (const auto& obj : m_SceneObjects)
			{
				if (typeid(Camera) == typeid(*obj))
				{
					return std::static_pointer_cast<Camera>(obj);
				}
			}

			return nullptr;
		}

		Ref<DirectionalLight> GetDirectionalLight() { return m_DirLight; }
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

		Ref<ViewportCamera> m_ViewportCamera;

		Ref<Camera> m_ActiveCamera;

		Ref<DirectionalLight> m_DirLight;

		std::vector<Ref<PointLight>> m_PointLights;
		std::vector<Ref<SpotLight>> m_SpotLights;

		friend class SceneSubsystem;
		friend class RendererSubsystem;
		friend class EditorSubsystem;
		friend class Engine;
	};
}