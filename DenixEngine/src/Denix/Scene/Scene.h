#pragma once

#include "GameObject.h"
#include "Object/Shapes/Shapes.h"
#include "Object/Light/LightObject.h"
#include "Camera.h"


namespace Denix
{
	class Asset;

	constexpr unsigned int MAX_POINT_LIGHTS = 100;
	constexpr unsigned int MAX_SPOT_LIGHTS = 100;

	// Basic Scene class
	class Scene: public BaseObject
	{
	public:

		Scene(const std::string& _name = "Scene") : BaseObject(ObjectInitializer(_name)),
			m_SceneName{ _name },
			m_ViewportCamera{ nullptr },
			m_ActiveCamera{ nullptr },
			m_DirLight{ nullptr }
		{
		}

		Scene(const Ref<Asset>& _sceneAsset);

		virtual ~Scene() = default;

		std::string GetSceneName() const { return m_SceneName; }

		virtual bool Load()
		{
			m_ViewportCamera = MakeRef<Camera>(ObjectInitializer("Viewport Camera"));

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
		virtual void Update(float _deltaTime)
		{
			for (const auto& gameObject : m_SceneObjects)
			{
				// Update the GameObject -  This will always be here
				gameObject->Update(_deltaTime);
				if (m_IsPlaying) gameObject->Update(_deltaTime);
			}
		}

		bool IsLoaded() const { return m_IsLoaded; }
		bool IsOpen() const { return m_IsOpen; }
		bool IsPlaying() const { return m_IsPlaying; }

		template <class T = GameObject, typename... Args>
		Ref<T> SpawnGameObject(Args&&... _args, const glm::vec3& _position = glm::vec3(0.0f), const glm::vec3& _rotation = glm::vec3(0.0f))
		{
			try
			{
				if (Ref<GameObject> obj = MakeRef<T>(std::forward<Args>(_args)...))
				{
					// Set Transform Component
					obj->m_TransformComponent->SetPosition(_position);
					obj->m_TransformComponent->SetRotation(_rotation);
					
					if (m_IsOpen)
					{
						obj->BeginScene();

						if (m_IsPlaying)
							obj->BeginPlay();
					}

					// Type Checking for lights
					if (typeid(PointLight) == typeid(*obj))
					{
						if (m_PointLights.size() < MAX_POINT_LIGHTS)
						{
							m_PointLights.push_back(CastRef<PointLight>(obj));
						}
						else
						{
							DE_LOG(LogScene, Warn, "Max Point Lights Reached")
						}
					}
					else if (typeid(SpotLight) == typeid(*obj))
					{
						if (m_SpotLights.size() < MAX_SPOT_LIGHTS)
						{
							m_SpotLights.push_back(CastRef<SpotLight>(obj));
						}
						else
						{
							DE_LOG(LogScene, Warn, "Max Spot Lights Reached")
						}
					}
					else if (typeid(DirectionalLight) == typeid(*obj))
					{
						// Check if the scene already has a directional light
						if (m_DirLight)
						{
							DE_LOG(LogEditor, Warn, "Scene already has a directional light")
						}
						m_DirLight = CastRef<DirectionalLight>(obj);
					}

					m_SceneObjects.push_back(std::move(obj));

					return CastRef<T>(obj);
				}
				DE_LOG(LogScene, Error, "Failed to create object of type: {}", typeid(T).name());
			}
			catch (const std::exception& e)
			{
				DE_LOG(LogScene, Error, "Failed to spawn GameObject: {}", e.what());
			}

			return nullptr;
		}
		
		void SpawnGameObject(const Ref<GameObject>& _obj);

		void RemoveSceneObject(const Ref<GameObject>& obj)
		{
			if (const auto it = std::ranges::find(m_SceneObjects, obj); it != m_SceneObjects.end())
			{
				m_SceneObjects.erase(it);
			}
		}

		float GetGravity() const { return m_Gravity; }
		float& GetGravity() { return m_Gravity; }

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

		template<class T>
		Ref<GameObject> GetGameObjectByClass()
		{
			for (const auto& obj : m_SceneObjects)
			{
				if (typeid(T) == typeid(*obj))
				{
					return obj;
				}
			}

			return nullptr;
		}
	protected:

		/** Name of the scene. Must be uniqiue */
		std::string m_SceneName;
		Ref<Asset> m_SceneAsset;
		
		/** determine if the engine is in editor or tool side mode.
		 * True if the scene is being played. False if in editor mode.
		 */
		bool m_IsPlaying = false;

		bool m_IsLoaded = false;

		bool m_IsOpen = false;

		bool m_RequestStop = false;

		bool m_RequestStart = false;

		/** Gravity of the scene */
		float m_Gravity = 9.81f;

		/** List of Objects in the scene */
		std::vector<Ref<GameObject>> m_SceneObjects;

		Ref<Camera> m_ViewportCamera;

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
