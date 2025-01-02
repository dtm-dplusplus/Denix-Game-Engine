#include "PhysicsComponent.h"

#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/Component/TransformComponent.h"

namespace Denix
{
    PhysicsComponent::PhysicsComponent(): Component(ObjectInit("Physics Component")), m_Collider(nullptr)
    {
		m_BroadCollider = MakeRef<SphereCollider>();
		m_BroadCollider->GetRadius() = 2.0f;
    }

    void PhysicsComponent::ComputeCenterOfMass()
    {
        // Compute the center of mass of the object
        // For now, we will assume the center of mass is at the center of the object
        m_CenterOfMass = m_Parent->m_TransformComponent->GetPosition();
    }

    void PhysicsComponent::ComputeObjectInertiaTensor()
    {
        glm::mat3 rotationMatrix = m_Parent->m_TransformComponent->m_RotationMatrix;
        m_ObjectInteriaTensor =rotationMatrix * m_BodyInteriaTensor * glm::transpose(rotationMatrix);
    }

    void PhysicsComponent::ComputeObjectInverseInertiaTensor()
    {
        glm::mat3 rotationMatrix = m_Parent->m_TransformComponent->m_RotationMatrix;
        m_ObjectInteriaTensorInverse = m_Parent->m_TransformComponent->m_RotationMatrix *
            m_BodyInteriaTensorInverse * glm::transpose(rotationMatrix);
    }

    void PhysicsComponent::ComputeBodyInertiaTensor()
    {
        // Set Inertia Tensor && inverse Inertia Tensor
        if (m_Collider)
        {
            switch (m_Collider->GetColliderType())
            {
            case ColliderType::Cube:
                {
                    Ref<CubeCollider> cubeCol = CastRef<CubeCollider>(m_Collider);

                } break;

            case ColliderType::Sphere:
                {
                    Ref<SphereCollider> sphereCol = CastRef<SphereCollider>(m_Collider);
                    m_BodyInteriaTensor = glm::mat3((2.0f / 5.0f) * m_Mass * pow(sphereCol->GetRadius(), 2));
                    m_BodyInteriaTensorInverse = glm::inverse(m_BodyInteriaTensor);
                } break;
            }
        }
    }

    void PhysicsComponent::ComputeRotationMatrix(float _deltaTime)
    {
        m_Parent->m_TransformComponent->m_RotationMatrix += GetSkewMatrix(m_AngularVelocity) * m_Parent->m_TransformComponent->m_RotationMatrix * _deltaTime;
    }

    void PhysicsComponent::ComputeStepEuler(float _deltaTime)
    {
        //////////////* Linear */////////////////////
		// Calculate acceleration at time t
		m_Acceleration = m_Force / m_Mass;

        // Calculate new velocity at time t + dt
        m_Velocity += m_Acceleration * _deltaTime;

        // Calculate new displacement at time t + dt
        m_Parent->m_TransformComponent->m_Position += m_Velocity * _deltaTime;

        ////////////* Angular */////////////////////
        if (!m_RotationEnabled) return;
        
        // Calulate angular momentum
        m_AngularMomentum += m_Torque * _deltaTime;

        // Calculate Inverse Inertia Tensor
        ComputeObjectInverseInertiaTensor();

        // Calulate angular velocity
        m_AngularVelocity = m_ObjectInteriaTensorInverse * m_AngularMomentum;

        // Reconstruct skew matrix
         /*glm::mat3 skewMatrix = glm::mat3(
             0.0f, -m_AngularVelocity.z, m_AngularVelocity.y,
             m_AngularVelocity.z, 0.0f, -m_AngularVelocity.x,
             -m_AngularVelocity.y, m_AngularVelocity.x, 0.0f);*/

        // Update rotation matrix
        ComputeRotationMatrix(_deltaTime);

        glm::vec3 angles = GetEulerAngles(m_Parent->m_TransformComponent->m_RotationMatrix);
        m_Parent->m_TransformComponent->GetRotation() += angles;
    }

    void PhysicsComponent::ComputeStepRK2(float _deltaTime)
    {
        //////////////* Linear */////////////////////
        m_Acceleration = m_Force / m_Mass;
        const glm::vec3 k1 = m_Acceleration * _deltaTime;

        // Calculate k2
        m_Force-= m_LinearDrag * (m_Velocity + k1);
        m_Acceleration = m_Force / m_Mass;
        const glm::vec3 k2 = m_Acceleration * _deltaTime;

        // Calculate new velocity at time t + dt
        m_Velocity += (k1 + k2) / 2.f;

        // Calculate new displacement at time t + dt
        m_Parent->m_TransformComponent->GetPosition() += m_Velocity * _deltaTime;
       
        ////////////* Angular */////////////////////
        // Calulate angular momentum
        m_AngularMomentum += m_Torque * _deltaTime;

        // Calculate Inverse Inertia Tensor
        ComputeObjectInverseInertiaTensor();

        // Calulate angular velocity
        m_AngularVelocity = m_ObjectInteriaTensorInverse * m_AngularMomentum;

        // Reconstruct skew matrix
        glm::mat3 skewMatrix = glm::mat3(
            0.0f, -m_AngularVelocity.z, m_AngularVelocity.y,
            m_AngularVelocity.z, 0.0f, -m_AngularVelocity.x,
            -m_AngularVelocity.y, m_AngularVelocity.x, 0.0f);

        // Update rotation matrix
        ComputeRotationMatrix(_deltaTime);

        glm::vec3 angles = GetEulerAngles(m_Parent->m_TransformComponent->m_RotationMatrix);
        m_Parent->m_TransformComponent->GetRotation() += angles;
    }

    Ref<Collider> PhysicsComponent::GetCollider() const
    { return m_Collider; }

    Ref<Collider>& PhysicsComponent::GetCollider()
    { return m_Collider; }

    void PhysicsComponent::SetCollider(const Ref<Collider>& _collider)
    {
        m_Collider = _collider;
    }

    void PhysicsComponent::BeginScene()
    {
        Component::BeginScene();
        
        RegisterComponent();
    }

    void PhysicsComponent::Update(float _deltaTime)
    {
        Component::Update(_deltaTime);

        
        
        /*
         *m_ParentTransform->m_PhysicsRotationOverride = m_SimulatePhysics;

        if (!m_Collider) return;
        
        m_Collider->m_TransformComponent->m_Position = m_ParentTransform->m_Position;

        switch (m_Collider->GetColliderType())
        {
        case ColliderType::Cube:
            {
                if (!m_CollisonDimesionOverride)
                    CastRef<CubeCollider>(m_Collider)->m_Dimensions= m_ParentTransform->m_Scale;
            } break;

        case ColliderType::Sphere:
            {
                if(const Ref<SphereCollider> sphereCol = CastRef<SphereCollider>(m_Collider))
               {
                    m_MomentOfInertia = 0.4f * m_Mass * (sphereCol->m_Radius * sphereCol->m_Radius);
                    m_ParentTransform->m_Scale = glm::vec3(sphereCol->m_Radius * 2.0f);
                    m_Collider->m_TransformComponent->m_Scale = m_ParentTransform->m_Scale;
               }
            } break;
        }
        */
    }

    void PhysicsComponent::StepSimulation(float _deltaTime)
    {
        if (!m_SimulatePhysics || m_SteppedThisFrame) return;
        
        // Calculate the net force - Null effect if Drag = 0
        m_Force -= m_LinearDrag * m_Velocity;
        m_Torque -= m_AngularDrag * m_AngularVelocity;

        ComputeStepEuler(_deltaTime);

        m_SteppedThisFrame = true;
    }

    void PhysicsComponent::BeginPlay()
    {
        // Register the physics component with the physics subsystem
        Component::BeginPlay();


        // Initialize the physics component
        m_CenterOfMass = m_Parent->m_TransformComponent->m_Position;
        m_PreviousPosition = m_Parent->m_TransformComponent->m_Position;
        m_Parent->m_TransformComponent->m_RotationMatrix =  glm::mat4(1.0f);

        m_Force = glm::vec3(0.0f);
        m_Torque = glm::vec3(0.0f);
        m_Acceleration = glm::vec3(0.0f);
        m_Velocity = glm::vec3(0.0f);
        m_AngularMomentum = glm::vec3(0.0f);
        m_AngularVelocity = glm::vec3(0.0f);
        ComputeBodyInertiaTensor();
        ComputeObjectInverseInertiaTensor();
        ComputeAngularVelocity();

    }

    void PhysicsComponent::EndScene()
    {
        UnregisterComponent();

        Component::EndScene();
    }

    void PhysicsComponent::RegisterComponent()
    {
            PhysicsSubsystem::RegisterComponent(shared_from_this());
    }

    void PhysicsComponent::UnregisterComponent()
    {
        PhysicsSubsystem::UnregisterComponent(shared_from_this());
    }

}
