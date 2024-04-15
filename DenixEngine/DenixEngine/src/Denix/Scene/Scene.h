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

			m_DirLight = MakeRef<DirectionalLight>();
			m_DirLight->GetTransformComponent()->SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));
			m_SceneObjects.push_back(m_DirLight);

			m_IsLoaded = true;

			return true;
		}

		virtual void Unload()
		{
			m_IsLoaded = false;
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

			m_IsOpen = true;
		}


		virtual void EndScene()
		{
			for (const auto& obj : m_SceneObjects)
			{
				obj->EndScene();
			}

			m_IsOpen = false;
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

			// Give camera back to viewport camera
			m_ActiveCamera = m_ViewportCamera;
		}
		virtual void Update(float _deltaTime) {}

		bool IsLoaded() const { return m_IsLoaded; }
		bool IsOpen() const { return m_IsOpen; }
		bool IsPlaying() const { return m_IsPlaying; }

		void SpawnSceneObject(const Ref<GameObject>& _object)
		{
			// An object can be spawned as long as the level is loaded. It doesn't have to be open
			if (!m_IsLoaded) 
			{
				DE_LOG(LogScene, Error, "Scene is not loaded. Failed To Spawn Object: {}", _object->GetName())
				return;
			}

			if (m_IsOpen)
			{
				_object->BeginScene();

				if (m_IsPlaying)
				{
					_object->BeginPlay();
				}
			}

			// Type Checking for lights
			if (typeid(PointLight) == typeid(*_object))
			{
				if (m_PointLights.size() < MAX_POINT_LIGHTS)
				{
					m_PointLights.push_back(std::dynamic_pointer_cast<PointLight>(_object));
				}
				else
				{
					DE_LOG(LogScene, Warn, "Max Point Lights Reached")
				}
			}
			else if (typeid(SpotLight) == typeid(*_object))
			{
				if (m_SpotLights.size() < MAX_SPOT_LIGHTS)
				{
					m_SpotLights.push_back(std::dynamic_pointer_cast<SpotLight>(_object));
				}
				else
				{
					DE_LOG(LogScene, Warn, "Max Spot Lights Reached")
				}
			}
			else if (typeid(DirectionalLight) == typeid(*_object))
			{
				// Check if the scene already has a directional light
				if (m_DirLight)
				{
					DE_LOG(LogEditor, Warn, "Scene already has a directional light")
					return;
				}
				m_DirLight = std::dynamic_pointer_cast<DirectionalLight>(_object);
			}
		
			m_SceneObjects.push_back(_object);
		}

		void RemoveSceneObject(const Ref<GameObject>& _object)
		{
			if (const auto it = std::ranges::find(m_SceneObjects, _object); it != m_SceneObjects.end())
			{
				m_SceneObjects.erase(it);
			}
		}

		glm::vec3 GetGravity() const { return m_SceneGravity; }

		Ref<ViewportCamera> GetViewportCamera() { return m_ViewportCamera; }

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
		void SetDirectionalLight(const Ref<DirectionalLight>& _dirLight) { m_DirLight = _dirLight; }

		std::vector<Ref<GameObject>> GetSceneObjects()const { return m_SceneObjects; }

		Ref<GameObject> GetGameObject(const std::string& _name) const
		{
			for (const auto& obj : m_SceneObjects)
			{
				if (obj->GetName() == _name)
				{
					return obj;
				}
			}

			return nullptr;
		}
	protected:

		/** Name of the scene. Must be uniqiue */
		std::string m_SceneName;

		/** determine if the engine is in editor or tool side mode.
		 * True if the scene is being played. False if in editor mode.
		 */
		bool m_IsPlaying = false;

		bool m_IsLoaded = false;

		bool m_IsOpen = false;

		/** Gravity of the scene */
		glm::vec3 m_SceneGravity = glm::vec3(0.0f, -9.81f, 0.0f);

		/** List of Objects in the scene */
		std::vector<Ref<GameObject>> m_SceneObjects;
		//std::unordered_map<std::string, Ref<GameObject>> m_SceneObjects;

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