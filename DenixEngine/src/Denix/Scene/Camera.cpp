#include "Camera.h"

#include "Denix/Input/InputSubsystem.h"

namespace Denix
{
	Camera::Camera(): Actor({"Camera"}), m_Viewport(MakeRef<Viewport>()), m_Projection(1.0f), m_View(1.0f)
	{
		m_TransformComponent->SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
		m_TransformComponent->SetPosition({0.0f, 10.0f, 25.0f});
		m_RenderComponent->SetIsVisible(false);
	}

	void Camera::Update(float _deltaTime)
	{
		// Camera Movement
		if (!m_ExternalControl)
		{
			ProcessKeyboardInput(_deltaTime);

			ProccessMouseMovement(_deltaTime);
		}
		
		Actor::Update(_deltaTime);

		const auto& fwd = m_TransformComponent->GetForward();
		const auto& up = m_TransformComponent->GetUp();
		
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
		m_View = glm::lookAt(m_TransformComponent->GetPosition(), m_TransformComponent->GetPosition() + fwd, up);
	}
		
	void Camera::ProcessKeyboardInput(float _deltaTime)
	{
		const auto& fwd = m_TransformComponent->GetForward();
		const auto& right = m_TransformComponent->GetRight();
		
		// XZ 
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_W))
		{
			m_TransformComponent->GetPosition() += m_MoveSpeed * fwd * _deltaTime;
		}
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_S))
		{
			m_TransformComponent->GetPosition() -= m_MoveSpeed * fwd * _deltaTime;
		}
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_A))
		{
			m_TransformComponent->GetPosition() -= m_MoveSpeed * right * _deltaTime;
		}
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_D))
		{
			m_TransformComponent->GetPosition() += m_MoveSpeed * right * _deltaTime;
		}
	}

	void Camera::ProccessMouseMovement(float _deltaTime)
	{
		const MouseData& mouseData = InputSubsystem::GetMouseData();
		
		// Pan Camera if right _mouse down
		if (m_EnableRotation && mouseData.Right && abs(mouseData.RelY) + abs(mouseData.RelX) > 0.1f)
		{
			m_TransformComponent->GetRotation().y += mouseData.RelX * m_YawRotationRate * _deltaTime * m_RotationFactor;
			m_TransformComponent->GetRotation().x -= mouseData.RelY * m_PitchRotationRate * _deltaTime * m_RotationFactor;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (m_TransformComponent->GetRotation().x > 89.0f)
				m_TransformComponent->GetRotation().x = 89.0f;
			else if (m_TransformComponent->GetRotation().x < -89.0f)
				m_TransformComponent->GetRotation().x = -89.0f;
		}

		// Change Height if middle _mouse down
		const float mouseRelMag = abs(mouseData.RelY) + abs(mouseData.RelX);
		if(mouseData.Middle &&  mouseRelMag > 0.1f)
		{
			m_TransformComponent->GetPosition().y -= mouseData.RelY * m_MoveSpeed * _deltaTime;
			m_TransformComponent->GetPosition().x += mouseData.RelX * m_MoveSpeed * _deltaTime;
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