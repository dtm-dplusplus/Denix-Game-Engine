#include "TransformComponent.h"
#include "Denix/System/SceneSubSystem.h"

namespace Denix
{
    void TransformComponent::RegisterComponent()
    {
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