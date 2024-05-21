#include "PhysicsComponent.h"

#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/Component/TransformComponent.h"

namespace Denix
{
    PhysicsComponent::PhysicsComponent(): Component(ObjectInitializer("Physics Component")), m_Collider(nullptr)
    {
        // Set Inertia Tensor && inverse Inertia Tensor
    }

    PhysicsComponent::PhysicsComponent(const std::string& _parentName): 
        Component(_parentName, ObjectInitializer("Physics Component")), m_Collider(nullptr)
    {
    }

    void PhysicsComponent::ComputeCenterOfMass()
    {
        // Compute the center of mass of the object
        // For now, we will assume the center of mass is at the center of the object
        m_CenterOfMass = m_ParentTransform->GetPosition();
    }

    void PhysicsComponent::ComputeRotationMatrix(float _deltaTime)
    {
        m_ParentTransform->m_RotationMatrix += GetSkewMatrix(m_AngularVelocity) * m_ParentTransform->m_RotationMatrix * _deltaTime;
    }

    void PhysicsComponent::ComputeStepEuler(float _deltaTime)
    {
        //////////////* Linear */////////////////////
		// Calculate acceleration at time t
		m_Acceleration = m_Force / m_Mass;

        // Calculate new velocity at time t + dt
        m_Velocity += m_Acceleration * _deltaTime;

        // Calculate new displacement at time t + dt
        m_ParentTransform->GetPosition() += m_Velocity * _deltaTime;

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
        //m_ParentTransform->UpdateRotationVectorFromMatrix();

        glm::vec3 angles = GetEulerAngles(m_ParentTransform->m_RotationMatrix);
        m_ParentTransform->GetRotation() += angles;
    }

    void PhysicsComponent::ComputeStepRK2(float _deltaTime)
    {
        glm::vec3 netF;
        glm::vec3 accel;

        // Calculate k1
        netF = m_Force - m_LinearDrag * m_Velocity;
        accel = netF / m_Mass;
        const glm::vec3 k1 = accel * _deltaTime;

        // Calculate k2
        netF = m_Force - m_LinearDrag * (m_Velocity + k1);
        accel = netF / m_Mass;
        const glm::vec3 k2 = accel * _deltaTime;

        // Calculate new velocity at time t + dt
        m_Velocity += (k1 + k2) / 2.f;

        //m_Velocity = m_Velocity.length() > m_MinimumVelocity ? m_Velocity : glm::vec3(0.0f);

        // Calculate new displacement at time t + dt
        m_ParentTransform->GetPosition() += m_Velocity * _deltaTime;
    }

    void PhysicsComponent::StepSimulation(float _deltaTime)
    {
        // Calculate the net force - Null effect if Drag = 0
        m_Force -= m_LinearDrag * m_Velocity;

        // Calcylate net torque
        if (m_Collider)
        {
            switch (m_Collider->GetColliderType())
            {
            case ColliderType::Cube:
            {
                m_Torque = glm::vec3(0.0f); // No torque for cubes currently
            } break;

            case ColliderType::Sphere:
            {
                //Ref<SphereCollider> sphereCol = CastRef<SphereCollider>(m_Collider);
                m_Torque-= m_AngularDrag * m_AngularVelocity;

            } break;
            }

            m_Collider->Update(_deltaTime);
        }


        switch (m_StepMethod)
        {
        case StepMethod::Euler:
            ComputeStepEuler(_deltaTime);
            break;

        case StepMethod::RK2:
            ComputeStepRK2(_deltaTime);
            break;

        case StepMethod::RK4:
            ComputeStepRK4(_deltaTime);
            break;

        case StepMethod::Verlet:
            ComputeStepVerlet(_deltaTime);
            break;

        default:; // assert here
        }

        m_SteppedThisFrame = true;
    }

    void PhysicsComponent::BeginPlay()
    {
        // Register the physics component with the physics subsystem
        Component::BeginPlay();
        RegisterComponent();


        // Initialize the physics component
        m_CenterOfMass = m_ParentTransform->GetPosition();
        m_PreviousPosition = m_ParentTransform->GetPosition();
        m_ParentTransform->m_RotationMatrix = glm::mat4(1.0f);

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
        if (PhysicsSubsystem* physicsSystem = PhysicsSubsystem::Get())
        {
            physicsSystem->RegisterComponent(shared_from_this());
        }
    }

    void PhysicsComponent::UnregisterComponent()
    {
        if (PhysicsSubsystem* physicsSystem = PhysicsSubsystem::Get())
        {
            physicsSystem->UnregisterComponent(shared_from_this());
        }
    }

}
