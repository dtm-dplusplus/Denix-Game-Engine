/**
 * @file TransformComponent.h
 * @brief Definition of the TransformComponent class
 */
#pragma once

#include "TransformPrimitive.h"
#include "Denix/Scene/Component.h"

namespace Denix
{
    /**
     * @class TransformComponent
     * @brief Represents a transform component that handles position, rotation, and scale.
     */
    class TransformComponent final : public Component
    {
    public:
        /**
         * @brief Default constructor for TransformComponent.
         */
        TransformComponent();

        /**
         * @brief Default destructor for TransformComponent.
         */
        ~TransformComponent() override = default;

        /**
         * @brief Updates the components matrices and vectors.
         * @param _deltaTime Time step for the update.
         * @param _waitCounter Reference to a Counter object.
         */
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        /** @brief Gets the transform. */
        Transform GetTransform() const { return m_Transform; }
        /** @brief Gets a modifiable reference to the transform. */
        Transform& GetTransform() { return m_Transform; }
        /** @brief Sets the transform. */
        void SetTransform(const Transform& _transform) { m_Transform = _transform; }

        /** @brief Gets the position. */
        glm::vec3 GetPosition() const { return m_Transform.Position; }
        /** @brief Gets a modifiable reference to the position. */
        glm::vec3& GetPosition() { return m_Transform.Position; }
        /** @brief Sets the position using individual coordinates. */
        void SetPosition(const float _x, const float _y, const float _z) { m_Transform.Position = { _x, _y, _z }; }
        /** @brief Sets the position using a vector. */
        void SetPosition(const glm::vec3& _position) { m_Transform.Position = _position; }

        /** @brief Gets the rotation. */
        glm::vec3 GetRotation() const { return m_Transform.Rotation; }
        /** @brief Gets a modifiable reference to the rotation. */
        glm::vec3& GetRotation() { return m_Transform.Rotation; }
        /** @brief Sets the rotation using a vector. */
        void SetRotation(const glm::vec3& _rotation) { m_Transform.Rotation = _rotation; }
        /** @brief Sets the rotation using individual coordinates. */
        void SetRotation(const float _x, const float _y, const float _z) { m_Transform.Rotation = { _x, _y, _z }; }

        /** @brief Gets the scale. */
        glm::vec3 GetScale() const { return m_Transform.Scale; }
        /** @brief Gets a modifiable reference to the scale. */
        glm::vec3& GetScale() { return m_Transform.Scale; }
        /** @brief Sets the scale using a vector. */
        void SetScale(const glm::vec3& _scale) { m_Transform.Scale = _scale; }
        /** @brief Sets the scale using individual coordinates. */
        void SetScale(const float _x, const float _y, const float _z) { m_Transform.Scale = { _x, _y, _z }; }

        /** @brief Gets the forward vector. */
        glm::vec3 GetForward() const { return m_Forward; }
        /** @brief Gets a modifiable reference to the forward vector. */
        glm::vec3& GetForward() { return m_Forward; }

        /** @brief Gets the right vector. */
        glm::vec3 GetRight() const { return m_Right; }
        /** @brief Gets a modifiable reference to the right vector. */
        glm::vec3& GetRight() { return m_Right; }

        /** @brief Gets the up vector. */
        glm::vec3 GetUp() const { return m_Up; }
        /** @brief Gets a modifiable reference to the up vector. */
        glm::vec3& GetUp() { return m_Up; }

        /** @brief Gets the model matrix. */
        glm::mat4 GetModel() const { return m_ModelMatrix; }
        /** @brief Gets a modifiable reference to the model matrix. */
        glm::mat4& GetModel() { return m_ModelMatrix; }

    private:
        /** @brief Serializes the component. */
        void Serialize(YAML::Emitter& _out) override;
        /** @brief Deserializes the component. */
        void Deserialize(const YAML::Node& _in) override;

        Transform m_Transform; ///< Stores the transform data.

        glm::vec3 m_Forward; ///< Stores the forward vector.
        glm::vec3 m_Right; ///< Stores the right vector.
        glm::vec3 m_Up; ///< Stores the up vector.

        glm::mat4 m_ModelMatrix; ///< Stores the model matrix.

        friend class Actor; ///< Grants Actor class access.
        friend class Scene; ///< Grants Scene class access.
        friend class SceneSubsystem; ///< Grants SceneSubsystem class access.
        friend class PhysicsComponent; ///< Grants PhysicsComponent class access.
        friend class PhysicsSubsystem; ///< Grants PhysicsSubsystem class access.
    };
}
