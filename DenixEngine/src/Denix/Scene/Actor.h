/**
 * @file Actor.h
 * @brief Definition of the Actor class in the Denix engine.
 */

#pragma once

#include <ranges>
#include "Denix/Scene/Component.h"
#include "Denix/Scene/Component/RenderComponent.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Scene/Component/PhysicsComponent.h"
#include "Denix/Scene/Component/ModelComponent.h"

namespace Denix
{
    /**
     * @class Actor
     * @brief Base class for all actors in the scene.
     *
     * An Actor represents an entity within the game scene. It can contain multiple components
     * that define its behavior and properties such as transformation, physics, rendering, and more.
     */
    class Actor : public BaseObject
    {
    public:
        /** @brief Default constructor. */
        Actor();

        /**
         * @brief Constructor with initialization parameters.
         * @param _object_init Object initialization data.
         */
        Actor(const ObjectInit& _objInit);

        /** @brief Default destructor. */
        ~Actor() override = default;

        /**
         * @brief Serializes the actor's data.
         * @param _out YAML emitter to write data.
         */
        void Serialize(YAML::Emitter& _out) override;

        /**
         * @brief Deserializes the actor's data.
         * @param _in YAML node containing actor data.
         */
        void Deserialize(const YAML::Node& _in) override;

        /**
         * @brief Adds a component to the actor.
         * @param _comp Reference to the component.
         */
        void AddComponent(const Ref<Component>& _comp);

        /**
         * @brief Creates and adds a new component of type T.
         * @tparam T Component type.
         * @tparam Args Variadic arguments for component constructor.
         * @param _args Arguments to forward to the component constructor.
         * @return Reference to the created component.
         */
        template <class T, typename... Args>
        Ref<T> AddComponent(Args&&... _args);

        /**
         * @brief Retrieves a component of type T from the actor.
         * @tparam T Component type.
         * @return Reference to the component if found, nullptr otherwise.
         */
        template <typename T>
        Ref<T> GetComponent() const;

        /** @brief Gets the scene the actor is part of. */
        Ref<Scene> GetScene() { return m_SceneRef.lock(); }

        /** @brief Retrieves all components attached to the actor. */
        std::vector<Ref<Component>>& GetComponents() { return m_Components; }

        /** @brief Retrieves a map of component names to component references. */
        std::unordered_map<std::string, Ref<Component>>& GetComponentMap() { return m_ComponentMap; }

        /** @brief Gets the transform component of the actor. */
        Ref<TransformComponent> GetTransformComponent() { return m_TransformComponent; }

        /** @brief Gets the transform data of the actor. */
        Transform& GetTransform() { return m_TransformComponent->GetTransform(); }

        /** @brief Gets the transform data of the actor (const version). */
        Transform GetTransform() const { return m_TransformComponent->GetTransform(); }

        /** @brief Gets the physics component of the actor. */
        Ref<PhysicsComponent> GetPhysicsComponent() { return m_PhysicsComponent; }

        /** @brief Gets the model component of the actor. */
        Ref<ModelComponent> GetModelComponent() { return m_ModelComponent; }

        /** @brief Gets the render component of the actor. */
        Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

        /**
         * @brief Updates the actor.
         * @param _deltaTime Time elapsed since last update.
         * @param _waitCounter Counter reference for synchronization.
         */
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        /** @brief Destroys the actor and its components. */
        void Destroy() override;

    protected:
        /** @brief Called when the scene begins. */
        void BeginScene() override;

        /** @brief Called when the scene ends. */
        void EndScene() override;

        /** @brief Called when the actor begins play. */
        void BeginPlay() override;

        /** @brief Called when the actor ends play. */
        void EndPlay() override;

        /**
         * @brief Handles collision entry event.
         * @param _other The other actor involved in the collision.
         * @param _normal The collision normal.
         * @param _point The point of collision.
         */
        virtual void OnCollisionEnter(const Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point) {}

        /**
         * @brief Handles collision exit event.
         * @param _other The other actor involved in the collision.
         * @param _normal The collision normal.
         * @param _point The point of collision.
         */
        virtual void OnCollisionExit(const Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point) {}

        Ref<TransformComponent> m_TransformComponent; ///< Actor's transform component.
        Ref<PhysicsComponent> m_PhysicsComponent; ///< Actor's physics component.
        Ref<ModelComponent> m_ModelComponent; ///< Actor's model component.
        Ref<RenderComponent> m_RenderComponent; ///< Actor's render component.

    private:
        std::unordered_map<std::string, Ref<Component>> m_ComponentMap; ///< Map of component names to components.
        std::vector<Ref<Component>> m_Components; ///< List of components attached to the actor.
        WRef<Scene> m_SceneRef; ///< Weak reference to the scene this actor belongs to.

        friend class Scene;
        friend class SceneSubsystem;
        friend class PhysicsSubsystem;
        friend class RendererSubsystem;
        friend class UISubsystem;
        friend class PhysicsComponent;
    };

    template <class T, typename... Args>
    Ref<T> Actor::AddComponent(Args&&... _args)
    {
        if (Ref<T> comp = MakeRef<T>(std::forward<Args>(_args)...))
        {
            AddComponent(comp);
            return comp;
        }

        return nullptr;
    }

    template <typename T>
    Ref<T> Actor::GetComponent() const
    {
        if (!m_ComponentMap.contains(ReflectionHelper::GetClassNameDE<T>()))
            return nullptr;

        if (Ref<T> component = CastRef<T>(m_ComponentMap.at(ReflectionHelper::GetClassNameDE<T>())))
            return component;

        return nullptr;
    }
}
