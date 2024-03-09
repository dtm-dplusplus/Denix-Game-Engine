#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GameObject.h"

namespace Denix
{
	class Camera: public GameObject
	{
	public:
		Camera(): GameObject(ObjectInitializer("Camera"))
		{
			m_TransformComponent->SetPosition(glm::vec3(0.0f, 0.0f, -25.f));
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
			View = glm::translate(glm::mat4(1.0f), m_TransformComponent->GetPosition());
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

		float MoveSpeed = 0.1f;
		bool IsPerspective = true;
		float Fov = 45.f;
		float NearPlane = 0.1f;
		float FarPlane = 10000.f;


		glm::mat4 Projection = glm::mat4(1.0f);
		glm::mat4 View = glm::mat4(1.0f);
		glm::vec2 Aspect = glm::vec2(800.f, 600.f);

		friend class SceneSubSystem;
	};
}
