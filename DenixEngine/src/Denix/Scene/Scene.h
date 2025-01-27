#pragma once

#include "Actor.h"
#include "Camera.h"

namespace physx
{
	class PxControllerManager;
	class PxSceneDesc;
	class PxScene;
}

namespace Denix
{
	class Asset;
	class CollisionCallback;

	// Basic Scene class
	class Scene: public BaseObject, public std::enable_shared_from_this<Scene>
	{
	public:
		Scene();

		~Scene() override;

		void BeginScene() override;

		void EndScene() override;

		void BeginPlay() override;

		void EndPlay() override;

		void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

		virtual void DebugUI(float _deltaTime, const Ref<Counter>& _waitCounter) {}

		bool IsPlaying() const;

		template <class T = Actor, typename... Args>
		Ref<T> SpawnActor(Args&&... _args, const glm::vec3& _position = glm::vec3(0.0f), const glm::vec3& _rotation = glm::vec3(0.0f), const glm::vec3& _scale = glm::vec3(1.0f));
		

		Ref<Camera> GetViewportCamera();

		Ref<Actor> GetActiveCamera();

		Ref<Actor> FindGameCamera() const;

		std::vector<Ref<Actor>> GetSceneActors() const;
		std::vector<Ref<Actor>>& GetSceneActors();

		Ref<Actor> GetActorByName(const std::string& _name) const;

		template<class T>
		Ref<Actor> GetActorByClass() const;

		template<class T>
		std::vector<Ref<Actor>> GetActorsOfClass() const;

		size_t GetActorCount() const { return m_Actors.size(); }

		// Debug Utility - Use with caution
		void ClearActors();

		physx::PxScene*	m_PxScene;
		physx::PxSceneDesc*		m_PxSceneDesc;
		physx::PxControllerManager*	m_PxControllerManager;
		CollisionCallback* m_CollisionCallback;
		/** Gravity of the scene */
		float m_Gravity = 9.81f;
		
	protected:
		/** Name of the scene. Must be uniqiue */
		Ref<Asset> m_SceneAsset;
		
		/** determine if the engine is in editor or tool side mode.
		 * True if the scene is being played. False if in editor mode.
		 */
		bool m_IsPlaying = false;

		Ref<Actor> m_GameCamera;
		
		Ref<Camera> m_ViewportCamera;

		Ref<Actor> m_ActiveCamera;

	private:
		void SpawnActor(const Ref<Actor>& _actor);

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
	Ref<T> Scene::SpawnActor(Args&&... _args, const glm::vec3& _position, const glm::vec3& _rotation, const glm::vec3& _scale)
{
	// Check if T is derived from Actor
	DE_STATIC_ASSERT(IsBase<Actor, T>(), "Class must be derived from Actor")

	if (Ref<Actor> actor = MakeRef<T>(std::forward<Args>(_args)...))
	{
		// Validate Name. We cannont have two objects with the same name
		if (m_ActorNames.contains(actor->GetName()))
		{
			int copy = 1;
			while (m_ActorNames.contains(actor->GetName() + std::to_string(copy))) copy++;
			actor->m_Name += std::to_string(copy);
		}
		m_ActorNames.insert(actor->m_Name);

		// Pass the scene reference to the actor
		actor->m_SceneRef = shared_from_this();
		
		// Set Transform Component
		actor->m_TransformComponent->m_Transform = Transform(_position, _rotation, _scale);
		
		// Run Begin Scene & Play. Implements any logic that needs to be run when the scene starts
		actor->BeginScene();
		if (m_IsPlaying) actor->BeginPlay();						

		// Add the object to the scene
		m_Actors.push_back(std::move(actor));
				
		// Retrun the actor reference as it's derived type
		return CastRef<T>(m_Actors.back());
	}
			
	DE_LOG(LogScene, Error, "Failed to create actor of type: {}", typeid(T).name());

	return nullptr;
}

	template <class T>
	Ref<Actor> Scene::GetActorByClass() const
	{
		for (const auto& actor : m_Actors)
			if (typeid(T) == typeid(*actor)) return actor;

		return nullptr;
	}

	template <class T>
	std::vector<Ref<Actor>> Scene::GetActorsOfClass() const
	{
		std::vector<Ref<Actor>> actors;

		for (const auto& actor : m_Actors)
			if (typeid(T) == typeid(*actor)) actors.push_back(actor);

		return actors;
	}
	
}
