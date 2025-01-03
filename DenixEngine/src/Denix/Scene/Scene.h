#pragma once

#include "Actor.h"
#include "Camera.h"

namespace physx
{
	class PxSceneDesc;
	class PxScene;
}

namespace Denix
{
	class Asset;

	// Basic Scene class
	class Scene: public BaseObject, public std::enable_shared_from_this<Scene>
	{
	public:
		Scene();

		~Scene() override;

		virtual bool Load();

		virtual void Unload();

		void BeginScene() override;

		void EndScene() override;

		void BeginPlay() override;

		void EndPlay() override;

		void Update(float _deltaTime) override;

		virtual void DebugUI(float _deltaTime);

		bool IsLoaded() const;
		bool IsOpen() const;
		bool IsPlaying() const;

		template <class T = Actor, typename... Args>
		Ref<T> SpawnActor(Args&&... _args, const glm::vec3& _position = glm::vec3(0.0f), const glm::vec3& _rotation = glm::vec3(0.0f));
		
		void SpawnActor(const Ref<Actor>& _obj);

		float GetGravity() const;
		float& GetGravity();

		Ref<Camera> GetViewportCamera();

		Ref<Camera> GetActiveCamera();

		Ref<Camera> GetGameCamera() const;

		std::vector<Ref<Actor>> GetSceneObjects() const;
		std::vector<Ref<Actor>>& GetSceneObjects();

		Ref<Actor> GetActorByName(const std::string& _name) const;

		template<class T>
		Ref<Actor> GetActorByClass() const;

		template<class T>
		std::vector<Ref<Actor>> GetActorsOfClass() const;

		size_t GetActorCount() const { return m_Actors.size(); }

		physx::PxScene*	m_PxScene;
		physx::PxSceneDesc*		m_PxSceneDesc;
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

		Ref<Camera> m_ViewportCamera;

		Ref<Camera> m_ActiveCamera;

		// Debug Utility - Use with caution
		void ClearScene();

	private:
		/** List of Objects in the scene */
		std::vector<Ref<Actor>> m_Actors;

		/**
		 * Map of actors in the scene
		 * Used to quickly find actors by name
		 */
		std::unordered_set<std::string> m_ActorNames;
		
		

		
		friend class SceneSubsystem;
		friend class RendererSubsystem;
		friend class EditorSubsystem;
		friend class Engine;
	};

	template <class T, typename... Args>
	Ref<T> Scene::SpawnActor(Args&&... _args, const glm::vec3& _position, const glm::vec3& _rotation)
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

	template <class T>
	Ref<Actor> Scene::GetActorByClass() const
	{
		for (const auto& obj : m_Actors)
			if (typeid(T) == typeid(*obj)) return obj;

		return nullptr;
	}

	template <class T>
	std::vector<Ref<Actor>> Scene::GetActorsOfClass() const
	{
		std::vector<Ref<Actor>> actors;

		for (const auto& obj : m_Actors)
			if (typeid(T) == typeid(*obj)) actors.push_back(obj);

		return actors;
	}
	
}
