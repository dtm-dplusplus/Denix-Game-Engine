#include "Camera.h"

#include "Denix/Input/InputSubsystem.h"

namespace Denix
{
	void Camera::Update(float _deltaTime)
	{
		// m_Projection matrix
		if (m_IsPerspective)
		{
			m_Projection = glm::perspective(glm::radians(m_Fov), m_Aspect.x / m_Aspect.y, m_NearPlane, m_FarPlane);
		}
		else
		{
			m_Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, m_NearPlane, m_FarPlane);
		}

		// Camera Movement
		ProcessKeyboardInput(_deltaTime);

		ProccessMouseMovement(_deltaTime);

		// calculates the front, right and up vectors
		glm::vec3 front;
		const glm::vec3& rot= m_TransformComponent->GetRotation();
		front.x = cos(glm::radians(rot.y)) * cos(glm::radians(rot.x));
		front.y = sin(glm::radians(rot.x));
		front.z = sin(glm::radians(rot.y)) * cos(glm::radians(rot.x));
		m_CameraFront = glm::normalize(front);

		m_CameraRight = glm::normalize(glm::cross(m_CameraFront, {0.0f, 1.0f, 0.0f})); // World up
		m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));

		// Calculate the view matrix
		m_View = glm::lookAt(m_TransformComponent->GetPosition(), m_TransformComponent->GetPosition() + m_CameraFront, m_CameraUp);
	}
		
	void Camera::ProcessKeyboardInput(float _deltaTime)
	{
		// XZ 
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_W))
		{
			GetTransformComponent()->GetPosition() += m_MoveSpeed * m_CameraFront * _deltaTime;
		}
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_S))
		{
			GetTransformComponent()->GetPosition() -= m_MoveSpeed * m_CameraFront * _deltaTime;
		}
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_A))
		{
			GetTransformComponent()->GetPosition() -= m_MoveSpeed * glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * _deltaTime;
		}
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_D))
		{
			GetTransformComponent()->GetPosition() += m_MoveSpeed * glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * _deltaTime;
		}
	}

	void Camera::ProccessMouseMovement(float _deltaTime)
	{
		const MouseData& mouseData = InputSubsystem::GetMouseData();
		
		// Pan Camera if right _mouse down
		if (mouseData.Right && abs(mouseData.RelY) + abs(mouseData.RelX) > 0.1f)
		{
			GetTransformComponent()->GetRotation().y += mouseData.RelX * m_YawRotationRate * _deltaTime * m_RotationFactor;
			GetTransformComponent()->GetRotation().x -= mouseData.RelY * m_PitchRotationRate * _deltaTime * m_RotationFactor;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (GetTransformComponent()->GetRotation().x > 89.0f)
				GetTransformComponent()->GetRotation().x = 89.0f;
			else if (GetTransformComponent()->GetRotation().x < -89.0f)
				GetTransformComponent()->GetRotation().x = -89.0f;
		}

		// Change Height if middle _mouse down
		const float mouseRelMag = abs(mouseData.RelY) + abs(mouseData.RelX);
		if(mouseData.Middle &&  mouseRelMag > 0.1f)
		{
			GetTransformComponent()->GetPosition().y -= mouseData.RelY * m_MoveSpeed * _deltaTime;
			GetTransformComponent()->GetPosition().x += mouseData.RelX * m_MoveSpeed * _deltaTime;
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
}