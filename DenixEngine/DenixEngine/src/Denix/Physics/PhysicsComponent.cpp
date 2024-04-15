#include "PhysicsComponent.h"

#include "Denix/Physics/Collider.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/Component/TransformComponent.h"

namespace Denix
{
    PhysicsComponent::PhysicsComponent(): Component(ObjectInitializer("Physics Component"))
    {
        m_Collider = MakeRef<Collider>();
        m_Collider->m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));
    }

    PhysicsComponent::PhysicsComponent(const std::string& _parentName): Component(_parentName, ObjectInitializer("Physics Component"))
    {
        m_Collider = MakeRef<Collider>();
        m_Collider->m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));
    }

    void PhysicsComponent::ComputeCenterOfMass()
    {
        // Compute the center of mass of the object
        // For now, we will assume the center of mass is at the center of the object
        m_CenterOfMass = m_ActorTransform->GetPosition();
    }

    void PhysicsComponent::Step(float _deltaTime)
    {
        // Step integration
        switch (m_StepMethod)
        {
        case StepMethod::Euler:
            StepEuler(_deltaTime);
            break;

        case StepMethod::RK2:
            StepRK2(_deltaTime);
            break;

        case StepMethod::RK4:
            StepRK4(_deltaTime);
            break;

        case StepMethod::Verlet:
            StepVerlet(_deltaTime);
            break;
        }
    }

    void PhysicsComponent::StepEuler(float _deltaTime)
    {
        // Calculate the net force - Null effect if Drag = 0
        m_Force -= m_Drag * m_Velocity;

        // Calculate new velocity at time t + dt
        m_Velocity += m_Force / m_Mass * _deltaTime;

        // Calculate new displacement at time t + dt
        m_ActorTransform->GetPosition() += m_Velocity * _deltaTime;
    }

    void PhysicsComponent::StepRK2(float _deltaTime)
    {
        glm::vec3 netF;
        glm::vec3 accel;

        // Calculate k1
        netF = m_Force - m_Drag * m_Velocity;
        accel = netF / m_Mass;
        const glm::vec3 k1 = accel * _deltaTime;

        // Calculate k2
        netF = m_Force - m_Drag * (m_Velocity + k1);
        accel = netF / m_Mass;
        const glm::vec3 k2 = accel * _deltaTime;

        // Calculate new velocity at time t + dt
        m_Velocity += (k1 + k2) / 2.f;

        // Calculate new displacement at time t + dt
        m_ActorTransform->GetPosition() += m_Velocity * _deltaTime;
    }

    void PhysicsComponent::BeginScene()
    {
        Component::BeginScene();

        RegisterComponent();
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
