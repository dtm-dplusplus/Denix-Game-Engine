#include "Camera.h"

#include "Denix/Input/InputSubsystem.h"

namespace Denix
{
	Camera::Camera(): Actor({"Camera"})
	{
		m_CameraComponent = AddComponent<CameraComponent>();
		GetTransformComponent()->SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
		GetTransformComponent()->SetPosition({0.0f, 1.0f, 5.0f});
		GetRenderComponent()->m_IsVisible = false;
	}

	void Camera::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
	{
		Actor::Update(_deltaTime, _waitCounter);
		
	}
}