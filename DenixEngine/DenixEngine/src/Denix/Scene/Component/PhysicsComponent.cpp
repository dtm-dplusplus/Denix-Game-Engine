#include "PhysicsComponent.h"
#include "Denix/System/PhysicsSubSystem.h"

void Denix::PhysicsComponent::RegisterComponent()
{
    if (PhysicsSubSystem* physicsSystem = PhysicsSubSystem::Get())
    {
        physicsSystem->RegisterComponent(shared_from_this());
    }
}

void Denix::PhysicsComponent::UnregisterComponent()
{
    if (PhysicsSubSystem* physicsSystem = PhysicsSubSystem::Get())
    {
        physicsSystem->UnregisterComponent(shared_from_this());
    }
}
