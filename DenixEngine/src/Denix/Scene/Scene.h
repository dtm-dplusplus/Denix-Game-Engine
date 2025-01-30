#pragma once

#include "Denix/Scene/Actor.h"
#include "Denix/Scene/Actor/Camera.h"

namespace physx
{
    class PxSceneDesc;
    class PxScene;
}

namespace Denix
{
    class Asset;
    class CollisionCallback;

    // Basic Scene class
    class Scene : public BaseObject
    {
    public:
        Scene();

        ~Scene() override;

        // Callend when scene is opened
        void BeginScene() override;

        // Called when scene is closed
        void EndScene() override;

        // Called when scene is played
        void BeginPlay() override;

        // Called when scene is stopped
        void EndPlay() override;

        // Called every frame
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        /* A seperated update call to present debug tools. 
        *
        * 
        */
        virtual void DebugUI(float _deltaTime, const Ref<Counter>& _waitCounter)
        {
        }

        // Returns scene state
        bool IsPlaying() const;

        /* Camera Utilities */
        Ref<Camera> GetViewportCamera();

        Ref<Actor> GetActiveCamera();

        // Search for a camera in the scene. Excludes the viewport camera
        Ref<Actor> FindGameCamera() const;

        /* Actor Utilities */
        /* Utility function to spawn an actor with transform in the scene 
        */
        template <class T = Actor, typename... Args>
        Ref<T> SpawnActor(Args&&... _args, const glm::vec3& _position = glm::vec3(0.0f),
                          const glm::vec3& _rotation = glm::vec3(0.0f), const glm::vec3& _scale = glm::vec3(1.0f));

        /* Adds Actor to Scene */
        void SpawnActor(const Ref<Actor>& _actor);

        // Copy of the actors in the scene
        std::vector<Ref<Actor>> GetSceneActors() const;

        // Get Actor by name
        Ref<Actor> GetActorByName(const std::string& _name) const;

        template <class T>
        Ref<Actor> GetActorByClass() const;

        template <class T>
        std::vector<Ref<Actor>> GetActorsOfClass() const;

        size_t GetActorCount() const { return m_Actors.size(); }

      

        /** Gravity of the scene */
        float m_Gravity = 9.81f;

    protected:
        // Debug Utility - Use with caution
        void ClearActors();

        /** Asset related to this scene. Contains offline data for this scene. */
        Ref<Asset> m_SceneAsset;

        /**
        * Useful flag managed by scene system. Used to ensure game logic is only executed when the scene is playing.
         */
        bool m_IsPlaying = false;

        /* Camera related members */
        /* If an actor containing a camera componet is found. It is stored here and activated on play. */
        Ref<Actor> m_GameCamera;

        /* Default camera for the scene */
        Ref<Camera> m_ViewportCamera;


        /* Active camera in the scene used for rendering. This can be viewport or game camera */
        Ref<Actor> m_ActiveCamera;

    private:
        /* Actor Containers 
        * We use two different containers. Vector is used for iteration and unordered_set is used for quick lookups.
        */
        /** List of Actors in the scene */
        std::vector<Ref<Actor>> m_Actors;

        /**
         * Map of actors in the scene
         * Used to quickly find actors by name
         */
        std::unordered_set<std::string> m_ActorNames;

        /* PhysX Members */
        /* PhysX Scene representation */
        physx::PxScene* m_PxScene;

        /* Callback Derived from PhysX Collision Callback to handle collision events */
        CollisionCallback* m_CollisionCallback;

        /* abstracts physx api from scene public API */
        friend class PhysicsSubsystem;
        friend class PhysicsComponent;

        friend class SceneSubsystem;
        friend class EditorSubsystem;
    };

    template <class T, typename... Args>
    Ref<T> Scene::SpawnActor(Args&&... _args, const glm::vec3& _position, const glm::vec3& _rotation,
                             const glm::vec3& _scale)
    {
        // Check if T is derived from Actor
        DE_STATIC_ASSERT(IsBase<Actor, T>(), "Class must be derived from Actor")

        if (Ref<Actor> actor = MakeRef<T>(std::forward<Args>(_args)...))
        {
            // Set Transfrom Data
            actor->m_TransformComponent->m_Transform = Transform(_position, _rotation, _scale);

            // Move the actor to the scene
            SpawnActor(actor);

            // Retrun the actor reference as it's derived type
            return CastRef<T>(actor);
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
