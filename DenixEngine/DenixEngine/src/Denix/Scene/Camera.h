#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GameObject.h"

namespace Denix
{
	class Camera : public GameObject
	{
	public:
		Camera() : GameObject(ObjectInitializer("Camera"))
		{
			m_TransformComponent->SetPosition(glm::vec3(0.0f, 0.0f, 100.f));
			m_TransformComponent->SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
		}

		~Camera() override = default;

		/*enum class Projection
		{
			Perspective,
			Orthographic
		};*/

		void Update(float _deltaTime) override
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
			//View = glm::translate(glm::mat4(1.0f), m_TransformComponent->GetPosition());
			//View = glm::rotate(View, glm::radians(m_TransformComponent->GetRotation().x), glm::vec3(1, 0, 0));	
			//View = glm::rotate(View, glm::radians(m_TransformComponent->GetRotation().y), glm::vec3(0, 1, 0));
			//View = glm::rotate(View, glm::radians(m_TransformComponent->GetRotation().z), glm::vec3(0, 0, 1));
			
			glm::vec3 direction;
			direction.x = cos(glm::radians(m_TransformComponent->GetRotation().y)) * cos(glm::radians(m_TransformComponent->GetRotation().x));
			direction.y = sin(glm::radians(m_TransformComponent->GetRotation().x));
			direction.z = sin(glm::radians(m_TransformComponent->GetRotation().y)) * cos(glm::radians(m_TransformComponent->GetRotation().x));
			CameraFront = glm::normalize(direction);

			View = glm::lookAt(m_TransformComponent->GetPosition(), m_TransformComponent->GetPosition() + CameraFront, CameraUp);
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
		//Projection m_ProjectionType = Projection::Perspective;

		// Camera Properties
		float MoveSpeed = 0.1f;
		float RotateSpeed = 10.0f;
		bool IsPerspective = true;
		float Fov = 45.f;
		float NearPlane = 0.1f;
		float FarPlane = 10000.f;

		glm::mat4 Projection = glm::mat4(1.0f);
		glm::mat4 View = glm::mat4(1.0f);
		glm::vec2 Aspect = glm::vec2(800.f, 600.f);

		glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		// Debug 
		bool m_IsPanMode = false;
		bool m_IsMoveSpeedDelta = false;

		friend class SceneSubsystem;
		friend class EditorSubsystem;

	};
}
