#include "Camera.h"

#include "Denix/Input/InputSubsystem.h"

namespace Denix
{
    Camera::Camera(): Actor({"Camera"})
    {
        m_CameraComponent = AddComponent<CameraComponent>();
        m_TransformComponent->SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
        m_TransformComponent->SetPosition({0.0f, 1.5f, 5.0f});
        m_RenderComponent->m_IsVisible = false;
        m_PhysicsComponent->IsEnabled() = false;
    }
}