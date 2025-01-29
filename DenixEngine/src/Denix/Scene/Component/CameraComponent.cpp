#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "CameraComponent.h"

#include "Denix/Scene/Actor.h"
#include "Denix/Scene/Camera.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Video/WindowSubsystem.h"

Denix::CameraComponent::CameraComponent(): Component({"CameraComponent"}), m_Viewport(MakeRef<Viewport>())
{
    m_ClassName = "CameraComponent";
}

void Denix::CameraComponent::BeginScene()
{
    Component::BeginScene();

    m_TransformComponent = m_Parent.lock()->GetComponent<TransformComponent>();
}

void Denix::CameraComponent::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Component::Update(_deltaTime, _waitCounter);


    // Camera Movement
    if (!m_ExternalControl)
    {
        ProcessKeyboardInput(_deltaTime, _waitCounter);

        ProccessMouseMovement(_deltaTime, _waitCounter);

        m_TransformComponent.lock()->Update(_deltaTime, _waitCounter); // Updating x2 for correct view - not ideal
    }

    auto transform = m_TransformComponent.lock();
    const auto& fwd = transform->GetForward();
    const auto& up = transform->GetUp();

    const glm::vec3& pos = transform->GetPosition();

    // Prepare Aspect
    m_Aspect = WindowSubsystem::GetWindowSize();
    m_AspectRatio = m_Aspect.x / m_Aspect.y;

    // m_Projection matrix
    if (m_IsPerspective)
    {
        m_Projection = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearPlane, m_FarPlane);
    }
    else
    {
        m_Projection = glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, m_NearPlane, m_FarPlane);
    }

    // Calculate the view matrix
    m_View = lookAt(pos, pos + fwd, up);
}

void Denix::CameraComponent::ProcessKeyboardInput(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    const auto& transform = m_TransformComponent.lock();
    glm::vec3& pos = transform->GetPosition();
    const glm::vec3& fwd = transform->GetForward();
    const glm::vec3& right = transform->GetRight();

    // XZ 
    if (InputSubsystem::IsKeyDown(KeyCode::DEK_W))
    {
        pos += m_MoveSpeed * fwd * _deltaTime;
    }
    if (InputSubsystem::IsKeyDown(KeyCode::DEK_S))
    {
        pos -= m_MoveSpeed * fwd * _deltaTime;
    }
    if (InputSubsystem::IsKeyDown(KeyCode::DEK_A))
    {
        pos -= m_MoveSpeed * right * _deltaTime;
    }
    if (InputSubsystem::IsKeyDown(KeyCode::DEK_D))
    {
        pos += m_MoveSpeed * right * _deltaTime;
    }
}

void Denix::CameraComponent::ProccessMouseMovement(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    const MouseData& mouseData = InputSubsystem::GetMouseData();

    const auto& transform = m_TransformComponent.lock();
    glm::vec3& pos = transform->GetPosition();
    glm::vec3& rot = transform->GetRotation();

    // Pan Camera if right _mouse down
    if (m_EnableRotation && mouseData.Right && abs(mouseData.RelY) + abs(mouseData.RelX) > 0.1f)
    {
        rot.y += mouseData.RelX * m_YawRotationRate * _deltaTime * m_RotationFactor;
        rot.x -= mouseData.RelY * m_PitchRotationRate * _deltaTime * m_RotationFactor;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        rot.x = glm::clamp(rot.x, -89.0f, 89.0f);
    }

    // Change Height if middle _mouse down
    const float mouseRelMag = abs(mouseData.RelY) + abs(mouseData.RelX);
    if (mouseData.Middle && mouseRelMag > 0.1f)
    {
        pos.y -= mouseData.RelY * m_MoveSpeed * _deltaTime;
        pos.x += mouseData.RelX * m_MoveSpeed * _deltaTime;
    }

    // Change move speed if _mouse wheel is scrolled
    if (mouseData.WheelY != 0)
    {
        m_MoveSpeed += mouseData.WheelY * m_MouseScrollSpeed;

        // Constrain the move speed
        m_MoveSpeed = glm::clamp(m_MoveSpeed, 0.5f, 100.0f);
    }
}
