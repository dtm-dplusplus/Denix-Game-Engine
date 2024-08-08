#include "Camera.h"

#include "Denix/Input/InputSubsystem.h"

void Denix::ViewportCamera::Update(float _deltaTime)
{
	Camera::Update(_deltaTime);

	// Camera Movement
	if (InputSubsystem* input = InputSubsystem::Get())
	{
		// XZ 
		if (input->IsKeyDown(SDL_SCANCODE_W))
		{
			GetTransformComponent()->GetPosition() += m_MoveSpeed * m_CameraFront * _deltaTime;
		}
		if (input->IsKeyDown(SDL_SCANCODE_S))
		{
			GetTransformComponent()->GetPosition() -= m_MoveSpeed * m_CameraFront * _deltaTime;
		}
		if (input->IsKeyDown(SDL_SCANCODE_A))
		{
			GetTransformComponent()->GetPosition() -= m_MoveSpeed * glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * _deltaTime;
		}
		if (input->IsKeyDown(SDL_SCANCODE_D))
		{
			GetTransformComponent()->GetPosition() += m_MoveSpeed * glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * _deltaTime;
		}

		// Camera Mouse Input
		const MouseData& mouse = input->GetMouseData();

		// Pan Camera if right mouse down
		if (mouse.Right && ((int)mouse.RelX || (int)mouse.RelY))
		{
			GetTransformComponent()->GetRotation().y += mouse.RelX * m_YawRotationRate * _deltaTime * m_RotationFactor;
			GetTransformComponent()->GetRotation().x -= mouse.RelY * m_PitchRotationRate * _deltaTime * m_RotationFactor;
		}

		// Change Height if middle mouse down
		if(mouse.Middle && (int)mouse.RelY)
		{
			GetTransformComponent()->GetPosition().y -= mouse.RelY * m_MoveSpeed * _deltaTime;
		}

		// Change move speed if mouse wheel is scrolled
		if (mouse.WheelY != 0)
		{
			m_MoveSpeed += mouse.WheelY * m_MouseScrollSpeed;
		}
	}
}
