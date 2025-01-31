/**
 * @file Scene.h
 * @brief Defines the Scene class and its actor utilities.
 */

#pragma once

#include "Denix/Scene/Actor/Camera.h"

namespace physx
{
    class PxScene;
}

namespace Denix
{
    class Asset;
    class CollisionCallback;

    /**
     * @class Scene
     * @brief Basic Scene class managing actors, cameras, and physics.
     */
    class Scene : public BaseObject
    {
    public:
        /**
         * @brief Constructor for Scene.
         */
        Scene();

        /**
         * @brief Destructor for Scene.
         */
        ~Scene() override;

        /**
         * @brief Called when the scene is opened.
         */
        void BeginScene() override;

        /**
         * @brief Called when the scene is closed.
         */
        void EndScene() override;

        /**
         * @brief Called when the scene starts playing.
         */
        void BeginPlay() override;

        /**
         * @brief Called when the scene stops playing.
         */
        void EndPlay() override;

        /**
         * @brief Called every frame to update the scene.
         * @param _deltaTime Time elapsed since the last frame.
         * @param _waitCounter Synchronization counter reference.
         */
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        /**
         * @brief Debug function for presenting debug tools. Enable ShowDebugToolsInPlay to show debug tools in play mode
         * @param _deltaTime Time elapsed since the last frame.
         * @param _waitCounter Synchronization counter reference.
         */
        virtual void ToolUpdate(float _deltaTime, const Ref<Counter>& _waitCounter)
        {
        }

        /**
         * @brief Checks if the scene is currently playing.
         * @return True if the scene is playing, otherwise false.
         */
        bool IsPlaying() const;

        /** @name Camera Utilities */
        ///@{
        /**
         * @brief Gets the viewport camera.
         * @return Reference to the viewport camera.
         */
        Ref<Camera> GetViewportCamera();

        /**
         * @brief Gets the active camera in the scene.
         * @return Reference to the active camera.
         */
        Ref<Actor> GetActiveCamera();

        /**
         * @brief Gets the active camera component in the scene.
         * @return Reference to the active camera component.
         */
        Ref<CameraComponent> GetActiveCameraComponent();

        /**
         * @brief Finds a game camera in the scene, excluding the viewport camera.
         * @return Reference to the found camera.
         */
        Ref<Actor> FindGameCamera() const;
        ///@}

        /** @name Actor Utilities */
        ///@{
        /**
         * @brief Spawns an actor with a specified transform in the scene.
         * @tparam T The type of actor to spawn.
         * @tparam Args Variadic template parameters for actor construction.
         * @param _args Arguments for constructing the actor.
         * @param _position Initial position of the actor.
         * @param _rotation Initial rotation of the actor.
         * @param _scale Initial scale of the actor.
         * @return Reference to the spawned actor.
         */
        template <class T = Actor, typename... Args>
        Ref<T> SpawnActor(Args&&... _args, const glm::vec3& _position = glm::vec3(0.0f),
            const glm::vec3& _rotation = glm::vec3(0.0f), const glm::vec3& _scale = glm::vec3(1.0f));

        /**
         * @brief Adds an actor to the scene.
         * @param _actor Reference to the actor to add.
         */
        void SpawnActor(const Ref<Actor>& _actor);

        /**
         * @brief Retrieves a copy of all actors in the scene.
         * @return Vector containing references to all actors.
         */
        std::vector<Ref<Actor>> GetSceneActors() const;

        /**
         * @brief Retrieves an actor by name.
         * @param _name Name of the actor to find.
         * @return Reference to the found actor.
         */
        Ref<Actor> GetActorByName(const std::string& _name) const;

        /**
         * @brief Retrieves an actor by class type.
         * @tparam T The class type of the actor.
         * @return Reference to the found actor.
         */
        template <class T>
        Ref<Actor> GetActorByClass() const;

        /**
         * @brief Retrieves all actors of a specific class type.
         * @tparam T The class type of actors to find.
         * @return Vector containing references to found actors.
         */
        template <class T>
        std::vector<Ref<Actor>> GetActorsOfClass() const;

        /**
         * @brief Retrieves the number of actors in the scene.
         * @return Number of actors.
         */
        size_t GetActorCount() const { return m_Actors.size(); }

        /** @brief Enables or disables debug UI in play mode. */
        bool ShowDebugToolsInPlay() const { return m_DebugToolsInPlay; }
        bool& ShowDebugToolsInPlay() { return m_DebugToolsInPlay; }
        ///@}

        float m_Gravity; ///< Gravity of the scene.

    protected:
        /**
         * @brief Clears all actors from the scene. Use with caution.
         */
        void ClearActors();

        bool m_DebugToolsInPlay; ///< Flag to show debug tools in play mode.

    private:
        Ref<Asset> m_SceneAsset; /**< Asset related to this scene. */
        bool m_IsPlaying; /**< Flag to track if the scene is playing. */

        /** Camera-related members. */
        Ref<Actor> m_GameCamera; /**< Stores the game camera when an actor with a camera component is found. */
        Ref<Camera> m_ViewportCamera; /**< Default viewport camera for the scene. */
        Ref<Actor> m_ActiveCamera; /**< The active camera used for rendering. */

        /** Actor storage. */
        std::vector<Ref<Actor>> m_Actors; /**< List of actors in the scene. */
        std::unordered_set<std::string> m_ActorNames; /**< Map for quick actor lookups by name. */

        /** PhysX physics engine members. */
        physx::PxScene* m_PxScene; /**< Pointer to the PhysX scene representation. */
        CollisionCallback* m_CollisionCallback; /**< Collision callback for handling physics events. */

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