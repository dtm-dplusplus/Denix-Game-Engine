/**
 * @file PhysicsComponent.h
 * @brief Definition of the PhysicsComponent class
 */
#pragma once

#include <PxRigidBody.h>
#include <foundation/PxTransform.h>

#include "Denix/Core.h"
#include "Denix/Core/Math/Math.h"

#include "Denix/Scene/Component.h"
#include "Denix/Physics/CollisionPair.h"
#include "Denix/Physics/PhysicPrimitive.h"

namespace physx
{
    class PxRigidActor;
    class PxShape;
}

namespace Denix
{
    /**
     * @class PhysicsComponent
     * @brief Manages physics interactions for an actor, including forces, collisions, and simulation properties.
     */
    class PhysicsComponent : public Component
    {
    public:
        /**
         * @brief Default constructor for PhysicsComponent.
         */
        PhysicsComponent();

        /**
         * @brief Default destructor for PhysicsComponent.
         */
        ~PhysicsComponent() override = default;

        /**
         * @brief Applies a force to the physics component.
         * @param _force The force vector to apply.
         */
        void AddForce(const glm::vec3& _force) const;

        /**
         * @brief Applies an acceleration to the physics component.
         * @param _acceleration The acceleration vector to apply.
         */
        void AddAcceleration(const glm::vec3& _acceleration) const;

        /**
         * @brief Applies an impulse to the physics component.
         * @param _impulse The impulse vector to apply.
         */
        void AddImpulse(const glm::vec3& _impulse) const;

        /**
         * @brief Applies torque to the physics component.
         * @param _torque The torque vector to apply.
         */
        void AddTorque(const glm::vec3& _torque) const;

        /** @brief Determines if the physics component should update simulation. */
        bool m_SimulatePhysics;

        /** @brief Determines if collision detection should be performed. */
        bool m_CollisionDetectionEnabled;

        bool m_ImpulseEnabled;
        PhysicsAttributeFlags m_AttributeFlags;

        float m_SlopCoefficient;
        ColliderType m_ColliderType;

        /////////////////////* Linear Properties *///////////////////////
        float m_Mass; ///< Mass of the object.
        glm::vec3 m_InertiaTensor; ///< Moment of inertia.
        float m_LinearDrag; ///< Linear drag force.
        float m_AngularDrag; ///< Angular drag force.
        float m_Elasticity; ///< Elasticity for impulse response (bounciness).
        float m_StaticFriction;
        float m_DynamicFriction;

        /////////////////////* Angular Properties *///////////////////////
        glm::vec3 m_AngularVelocity; ///< Angular velocity.
        glm::vec3 m_AngularMomentum; ///< Angular momentum.

        /////////////////////* Read-Only Properties *///////////////////////
        glm::vec3 m_Velocity; ///< Velocity of the object.
        glm::vec3 m_Acceleration; ///< Acceleration of the object.
        glm::vec3 m_Force; ///< Force acting on the object.
        glm::vec3 m_Torque; ///< Net torque.

    private:
        /** @brief Sets the inertia tensor based on the object's properties. */
        void SetInertia();

        void RegisterComponent() override;
        void UnregisterComponent() override;

        /** @brief Serializes the component data. */
        void Serialize(YAML::Emitter& _out) override;

        /** @brief Deserializes the component data. */
        void Deserialize(const YAML::Node& _in) override;

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
        void PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        std::vector<CollisionPair> m_CollisionData; ///< Collision event container for actor collison events 

        physx::PxShape* m_PxShape; ///< PhysX shape pointer.
        physx::PxRigidActor* m_PxActor; ///< PhysX actor pointer.
        physx::PxMaterial* m_PxMaterial; ///< PhysX material pointer.

        friend class EditorSubsystem;
        friend class SceneSubsystem;
        friend class PhysicsSubsystem;
        friend class CollisionCallback;
        friend class Actor;
    };
}
