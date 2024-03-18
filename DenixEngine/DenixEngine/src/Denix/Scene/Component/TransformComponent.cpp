#include "TransformComponent.h"
#include "Denix/System/SceneSubSystem.h"

namespace Denix
{
    void TransformComponent::BeginScene()
    {
        Component::BeginScene();

        RegisterComponent();
    }

    void TransformComponent::EndScene()
    {
        UnregisterComponent();
    
        Component::EndScene();
    }

    void TransformComponent::RegisterComponent()
    {
        Component::RegisterComponent();

        if (SceneSubSystem* sceneSystem = SceneSubSystem::Get())
        {
            sceneSystem->RegisterTransformComponent(shared_from_this());
        }
    }

    void TransformComponent::UnregisterComponent()
    {
        if (SceneSubSystem* sceneSystem = SceneSubSystem::Get())
        {
            sceneSystem->UnregisterTransformComponent(shared_from_this());
        }
    }

}