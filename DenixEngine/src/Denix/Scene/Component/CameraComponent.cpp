#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "CameraComponent.h"

#include "Denix/Scene/Actor.h"
#include "Denix/Scene/Camera.h"
#include "Denix/Input/InputSubsystem.h"

Denix::CameraComponent::CameraComponent(): Component({"CameraComponent"})
{
}

void Denix::CameraComponent::BeginScene()
{
    Component::BeginScene();

    m_TransformComponent = m_Parent.lock()->GetComponent<TransformComponent>();
}

void Denix::CameraComponent::Update(float _deltaTime)
{
    Component::Update(_deltaTime);

    // Camera Movement
    if (!m_ExternalControl)
    {
        ProcessKeyboardInput(_deltaTime);

        ProccessMouseMovement(_deltaTime);
    }

	auto transform = m_TransformComponent.lock();
	
    m_CameraFront = transform->GetForward();
    m_CameraRight = transform->GetRight();
    m_CameraUp = transform->GetUp();
	const glm::vec3& pos = transform->GetPosition();
    // m_Projection matrix
    if (m_IsPerspective)
    {
        m_Projection = glm::perspective(glm::radians(m_Fov), m_Aspect.x / m_Aspect.y, m_NearPlane, m_FarPlane);
    }
    else
    {
        m_Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, m_NearPlane, m_FarPlane);
    }
		
    // Calculate the view matrix
    m_View = glm::lookAt(pos, pos + m_CameraFront, m_CameraUp);

}

void Denix::CameraComponent::ProcessKeyboardInput(float _deltaTime)
	{
		glm::vec3& pos = m_TransformComponent.lock()->GetPosition();
	
		// XZ 
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_W))
		{
			pos += m_MoveSpeed * m_CameraFront * _deltaTime;
		}
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_S))
		{
			pos -= m_MoveSpeed * m_CameraFront * _deltaTime;
		}
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_A))
		{
			pos -= m_MoveSpeed * glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * _deltaTime;
		}
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_D))
		{
			pos += m_MoveSpeed * glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * _deltaTime;
		}
	}

	void Denix::CameraComponent::ProccessMouseMovement(float _deltaTime)
	{
		const MouseData& mouseData = InputSubsystem::GetMouseData();

	glm::vec3& pos = m_TransformComponent.lock()->GetPosition();
	glm::vec3& rot = m_TransformComponent.lock()->GetRotation();
	
		// Pan Camera if right _mouse down
		if (m_EnableRotation && mouseData.Right && abs(mouseData.RelY) + abs(mouseData.RelX) > 0.1f)
		{
			rot.y += mouseData.RelX * m_YawRotationRate * _deltaTime * m_RotationFactor;
			rot.x -= mouseData.RelY * m_PitchRotationRate * _deltaTime * m_RotationFactor;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (rot.x > 89.0f)
				rot.x = 89.0f;
			else if (rot.x < -89.0f)
				rot.x = -89.0f;
		}

		// Change Height if middle _mouse down
		const float mouseRelMag = abs(mouseData.RelY) + abs(mouseData.RelX);
		if(mouseData.Middle &&  mouseRelMag > 0.1f)
		{
			pos.y -= mouseData.RelY * m_MoveSpeed * _deltaTime;
			pos.x += mouseData.RelX * m_MoveSpeed * _deltaTime;
		}

		// Change move speed if _mouse wheel is scrolled
		if (mouseData.WheelY != 0)
		{
			m_MoveSpeed += mouseData.WheelY * m_MouseScrollSpeed;

			// Constrain the move speed
			if (m_MoveSpeed < 1.0f)
				m_MoveSpeed = 1.0f;
			else if (m_MoveSpeed > 100.0f)
				m_MoveSpeed = 50.0f;
		}
	}