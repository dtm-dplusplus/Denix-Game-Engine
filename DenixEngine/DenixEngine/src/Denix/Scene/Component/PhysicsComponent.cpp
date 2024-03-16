#include "PhysicsComponent.h"
#include "Denix/System/PhysicsSubSystem.h"

namespace Denix
{
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
        if (PhysicsSubSystem* physicsSystem = PhysicsSubSystem::Get())
        {
            physicsSystem->RegisterComponent(shared_from_this());
        }
    }

    void PhysicsComponent::UnregisterComponent()
    {
        if (PhysicsSubSystem* physicsSystem = PhysicsSubSystem::Get())
        {
            physicsSystem->UnregisterComponent(shared_from_this());
        }
    }

}
