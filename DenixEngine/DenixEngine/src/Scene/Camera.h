#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GameObject.h"

class Camera: public GameObject
{
public:
	Camera(): GameObject(ObjectInitializer("Camera"))
	{
		m_TransformComponent->SetPosition(glm::vec3(0.0f, 0.0f, -10.f));
	}
	~Camera() override = default;

	void Update() override
	{
		// Projection matrix
		if (IsPerspective)
		{
			Projection = glm::perspective(glm::radians(Fov), Aspect.x / Aspect.y, NearPlane, FarPlane);
		}
		else
		{
			Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, NearPlane, FarPlane);
		}

		// View Matrix
	}

	glm::mat4 GetProjectionMatrix() const
	{
		return Projection;
	}

	glm::mat4 GetViewMatrix() const
	{
		return View;
	}

protected:
	// Camera
	bool IsPerspective = true;
	float Fov = 45.f;
	float NearPlane = 0.1f;
	float FarPlane = 100.f;

	glm::mat4 Projection = glm::mat4(1.0f);
	glm::mat4 View = glm::mat4(1.0f);
	glm::vec2 Aspect = glm::vec2(800.f, 600.f);

	friend class SceneSubSystem;
	friend class DefaultScene;
};
