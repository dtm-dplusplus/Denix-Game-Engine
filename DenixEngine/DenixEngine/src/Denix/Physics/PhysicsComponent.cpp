#include "PhysicsComponent.h"

#include "Denix/Physics/Collider.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/Component/TransformComponent.h"

namespace Denix
{
    PhysicsComponent::PhysicsComponent(): Component(ObjectInitializer("Physics Component"))
    {
        m_Collider = MakeRef<CubeCollider>();
        m_Collider->m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));
    }

    PhysicsComponent::PhysicsComponent(const std::string& _parentName): Component(_parentName, ObjectInitializer("Physics Component"))
    {
        m_Collider = MakeRef<CubeCollider>();
        m_Collider->m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));
    }

    void PhysicsComponent::ComputeStepEuler(float _deltaTime)
    {
        // Calculate the net force - Null effect if Drag = 0
        m_Force -= m_LinearDrag * m_Velocity;


		// Calculate acceleration at time t
		m_Acceleration = m_Force / m_Mass;

        // Calculate new velocity at time t + dt
        m_Velocity += m_Acceleration * _deltaTime;

		//m_Velocity = m_Velocity.length() < m_MinimumVelocity? glm::vec3(0.0f) : m_Velocity;

        // Calculate new displacement at time t + dt
        m_ActorTransform->GetPosition() += m_Velocity * _deltaTime;
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

        m_Velocity = m_Velocity.length() > m_MinimumVelocity ? m_Velocity : glm::vec3(0.0f);

        // Calculate new displacement at time t + dt
        m_ActorTransform->GetPosition() += m_Velocity * _deltaTime;
    }

    void PhysicsComponent::BeginScene()
    {
        Component::BeginScene();

        RegisterComponent();

        const float tensor = (2.0f, 5.0f)  * m_Mass* pow(m_Radius, 2);
        glm::mat3 bodyInertiaTensor = {
            tensor, 0.0f, 0.0f,
			0.0f, tensor, 0.0f,
			0.0f, 0.0f, tensor
		};

        m_BodyInteriaTensorInverse = glm::inverse(bodyInertiaTensor);
		ComputeInverseInertiaTensor();

    /*   
        5) Pre - compute the body inertia tensor at rest body,
        and pre - compute the inverse of the body inertia tensor Ibody1;
        6) Initialize the position and velocity of the com, x and v, initialize the rigid body's rotation matrix
        R as the identity matrix and the angular momentum L = (0, 0, 0);
        7) Compute the inverse inertia tensor at the start : I1 = R Ibody" 8) Compute the angular velocity at the start: w(0) = 1-1 L ; */
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
