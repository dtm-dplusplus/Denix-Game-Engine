#include "TransformComponent.h"
#include "Denix/System/SceneSubsystem.h"

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

        if (SceneSubsystem* sceneSystem = SceneSubsystem::Get())
        {
            sceneSystem->RegisterTransformComponent(shared_from_this());
        }
    }

    void TransformComponent::UnregisterComponent()
    {
        if (SceneSubsystem* sceneSystem = SceneSubsystem::Get())
        {
            sceneSystem->UnregisterTransformComponent(shared_from_this());
        }
    }

}