#include "Camera.h"

#include "Denix/Input/InputSubsystem.h"

namespace Denix
{
	Camera::Camera(): Actor({"Camera"})
	{
		m_CameraComponent = AddComponent<CameraComponent>();
		m_TransformComponent->SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
		m_TransformComponent->SetPosition({0.0f, 10.0f, 25.0f});
		m_RenderComponent->SetIsVisible(false);
	}

	void Camera::Update(float _deltaTime)
	{
		Actor::Update(_deltaTime);
		
	}
}