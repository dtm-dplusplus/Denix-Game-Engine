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
    class PhysicsComponent : public Component
    {
    public:
        PhysicsComponent();

        ~PhysicsComponent() override = default;

        void AddForce(const glm::vec3& _force) const;

        void AddAcceleration(const glm::vec3& _acceleration) const;

        void AddImpulse(const glm::vec3& _impulse) const;

        void AddTorque(const glm::vec3& _torque) const;

        /** Set to decide if the physics component should update simulation */
        bool m_SimulatePhysics;

        /** Set to decide if the physics component should perform collision detection */
        bool m_CollisionDetectionEnabled;

        bool m_ImpulseEnabled;
        
        PhysicsAttributeFlags m_AttributeFlags;

        float m_SlopCoefficient;

        ColliderType m_ColliderType;

        /////////////////////* Linear Properties *///////////////////////
        /** Mass of the object */
        float m_Mass;

        /** Mass Moment of inertia of the object */
        glm::vec3 m_InertiaTensor;

        /** Linear Drag force acting on the object */
        float m_LinearDrag;

        /** Angular Drag force acting on the object */
        float m_AngularDrag;

        /** Elasticity used for impulse response (Bounciness) */
        float m_Elasticity;
        float m_StaticFriction;
        float m_DynamicFriction;

        /////////////////////* Angular Properties *///////////////////////
        /** Angular velocity of the object */
        glm::vec3 m_AngularVelocity;

        /** Angular Momentum */
        glm::vec3 m_AngularMomentum;

        /////////////////////* Read Only Properties *///////////////////////
        /** Velocity of the object */
        glm::vec3 m_Velocity;

        /** Acceleration of the object */
        glm::vec3 m_Acceleration;

        /** Force acting on the object */
        glm::vec3 m_Force;

        // Net Torque
        glm::vec3 m_Torque;

    private:
        void SetInertia();

        void RegisterComponent() override;
        void UnregisterComponent() override;

        void Serialize(YAML::Emitter& _out) override;
        void Deserialize(const YAML::Node& _in) override;

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
        void PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        void BeginScene() override;
        void EndScene() override;

        std::vector<CollisionPair> m_CollisionData;

        physx::PxShape* m_PxShape = nullptr;
        physx::PxRigidActor* m_PxActor = nullptr;
        physx::PxMaterial* m_PxMaterial = nullptr;

        friend class EditorSubsystem;
        friend class SceneSubsystem;
        friend class PhysicsSubsystem;
        friend class CollisionCallback;
        friend class Actor;
    };
}
