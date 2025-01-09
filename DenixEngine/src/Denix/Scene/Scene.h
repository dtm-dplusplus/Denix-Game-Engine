#pragma once

#include "Actor.h"
#include "Object/Shapes/Shapes.h"
#include "Object/Light/LightObject.h"
#include "Camera.h"
#include "Denix/Profile/ProfileSubsystem.h"


namespace Denix
{
	class Asset;

	constexpr unsigned int MAX_POINT_LIGHTS = 100;
	constexpr unsigned int MAX_SPOT_LIGHTS = 100;

	// Basic Scene class
	class Scene: public BaseObject, public std::enable_shared_from_this<Scene>
	{
	public:

		Scene() = default;
		Scene(const ObjectInit& _objInit);

		virtual ~Scene() = default;

		virtual bool Load()
		{
			m_ViewportCamera = MakeRef<Camera>(ObjectInit("Viewport Camera"));

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

			for (const auto& obj : m_Actors)
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
			for (const auto& obj : m_Actors)
			{
				obj->EndScene();
			}

			m_IsOpen = false;
		}

		virtual void BeginPlay()
		{
			m_IsPlaying = true;

			for (const auto& obj : m_Actors)
			{
				obj->BeginPlay();
			}
		}

		virtual void EndPlay()
		{
			m_IsPlaying = false;

			for (const auto& obj : m_Actors)
			{
				obj->EndPlay();
			}

			// Give camera back to viewport camera
			m_ActiveCamera = m_ViewportCamera;
		}

		void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override
		{
			BaseObject::Update(_deltaTime, _waitCounter);
			DebugUI(_deltaTime);
		}

		virtual void DebugUI(float _deltaTime){}

		bool IsLoaded() const { return m_IsLoaded; }
		bool IsOpen() const { return m_IsOpen; }
		bool IsPlaying() const { return m_IsPlaying; }

		template <class T = Actor, typename... Args>
		Ref<T> SpawnActor(Args&&... _args, const glm::vec3& _position = glm::vec3(0.0f), const glm::vec3& _rotation = glm::vec3(0.0f))
		{
			// Check if T is derived from Actor
			static_assert(std::is_base_of_v<Actor, T>, "T must be derived from Actor");

			if (Ref<Actor> obj = MakeRef<T>(std::forward<Args>(_args)...))
			{
				// Validate Name. We cannont have two objects with the same name
				if (m_ActorNames.contains(obj->GetName()))
				{
					int copy = 1;
					while (m_ActorNames.contains(obj->GetName() + std::to_string(copy))) copy++;
					obj->m_Name += std::to_string(copy);
				}
				m_ActorNames.insert(obj->m_Name);

				// Set Transform Component
				obj->m_TransformComponent->m_Position = _position;
				obj->m_TransformComponent->m_Rotation = _rotation;

				// Run Begin Scene & Play. Implements any logic that needs to be run when the scene starts
				obj->BeginScene();
				if (m_IsPlaying) obj->BeginPlay();						

				// Add the object to the scene
				m_Actors.push_back(std::move(obj));
				
				// Retrun the actor reference as it's derived type
				return CastRef<T>(m_Actors.back());
			}
			
			DE_LOG(LogScene, Error, "Failed to create object of type: {}", typeid(T).name());

			return nullptr;
		}
		
		void SpawnActor(const Ref<Actor>& _obj);

		void RemoveSceneObject(const Ref<Actor>& obj)
		{
			if (const auto it = std::ranges::find(m_Actors, obj); it != m_Actors.end())
			{
				m_Actors.erase(it);
			}
		}

		float GetGravity() const { return m_Gravity; }
		float& GetGravity() { return m_Gravity; }

		Ref<Camera> GetViewportCamera() { return m_ViewportCamera; }

		Ref<Camera> GetActiveCamera() { return m_ActiveCamera; }

		Ref<Camera> GetGameCamera() const
		{
			for (const auto& obj : m_Actors)
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

		std::vector<Ref<Actor>> GetSceneObjects() const { return m_Actors; }
		std::vector<Ref<Actor>>& GetSceneObjects() { return m_Actors; }
		
		Ref<Actor> GetActorByName(const std::string& _name) const
		{
			for (const auto& obj : m_Actors)
			{
				if (obj->GetName() == _name)
				{
					return obj;
				}
			}

			return nullptr;
		}

		template<class T>
		Ref<Actor> GetActorByClass()
		{
			for (const auto& obj : m_Actors)
			{
				if (typeid(T) == typeid(*obj))
				{
					return obj;
				}
			}

			return nullptr;
		}

		template<class T>
		std::vector<Ref<Actor>> GetActorsOfClass()
		{
			std::vector<Ref<Actor>> actors;

			for (const auto& obj : m_Actors)
			{
				if (typeid(T) == typeid(*obj))
				{
					actors.push_back(obj);
				}
			}

			return actors;
		}


		/**
		 * Map of actors in the scene
		 * Used to quickly find actors by name
		 */
		std::unordered_set<std::string> m_ActorNames;
		
	protected:

		/** Name of the scene. Must be uniqiue */
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
		std::vector<Ref<Actor>> m_Actors;

		
		
		Ref<Camera> m_ViewportCamera;

		Ref<Camera> m_ActiveCamera;

		Ref<DirectionalLight> m_DirLight;

		std::vector<Ref<PointLight>> m_PointLights;
		std::vector<Ref<SpotLight>> m_SpotLights;

	private:
		void ClearScene();

		
		friend class SceneSubsystem;
		friend class RendererSubsystem;
		friend class EditorSubsystem;
		friend class Engine;
	};
}
