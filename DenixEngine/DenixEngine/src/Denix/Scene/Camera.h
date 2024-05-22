#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Math.h"
#include "GameObject.h"
#include "Denix/Video/GL/Viewport.h"


namespace Denix
{
	class Camera : public GameObject
	{
	public:
		Camera(const ObjectInitializer& _objInit = {"Camera"}) : GameObject(ObjectInitializer(_objInit))
		{
			m_TransformComponent->SetPosition(glm::vec3(0.0f, 0.0f, 5));
			m_TransformComponent->SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
			m_RenderComponent->SetIsVisible(false);
		}

		~Camera() override = default;

		void Update(float _deltaTime) override
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

			// m_View Matrix
			glm::vec3 direction;
			direction.x = cos(glm::radians(m_TransformComponent->GetRotation().y)) * cos(glm::radians(m_TransformComponent->GetRotation().x));
			direction.y = sin(glm::radians(m_TransformComponent->GetRotation().x));
			direction.z = sin(glm::radians(m_TransformComponent->GetRotation().y)) * cos(glm::radians(m_TransformComponent->GetRotation().x));
			m_CameraFront = glm::normalize(direction);

			m_View = glm::lookAt(m_TransformComponent->GetPosition(), m_TransformComponent->GetPosition() + m_CameraFront, m_CameraUp);
		}

		bool IsPerspective() const
		{
			return m_IsPerspective;
		}
		bool& IsPerspective()
		{
			return m_IsPerspective;
		}
		void SetIsPerspective(const bool _isPerspective)
		{
			m_IsPerspective = _isPerspective;
		}

		float GetFov() const
		{
			return m_Fov;
		}
		float& GetFov()
		{
			return m_Fov;
		}
		void SetFov(const float _fov)
		{
			m_Fov = _fov;
		}

		float GetNearPlane() const
		{
			return m_NearPlane;
		}
		float& GetNearPlane()
		{
			return m_NearPlane;
		}
		void SetNearPlane(const float _nearPlane)
		{
			m_NearPlane = _nearPlane;
		}

		float GetFarPlane() const
		{
			return m_FarPlane;
		}
		float& GetFarPlane()
		{
			return m_FarPlane;
		}
		void SetFarPlane(const float _farPlane)
		{
			m_FarPlane = _farPlane;
		}

		glm::vec2 GetPitchConstraints() const
		{
			return m_PitchConstraints;
		}
		glm::vec2& GetPitchConstraints()
		{
			return m_PitchConstraints;
		}
		void SetPitchConstraints(const glm::vec2 _pitchConstraints)
		{
			m_PitchConstraints = _pitchConstraints;
		}

		glm::vec2 GetAspect() const
		{
			return m_Aspect;
		}
		glm::vec2& GetAspect()
		{
			return m_Aspect;
		}
		void SetAspect(const glm::vec2 _aspect)
		{
			m_Aspect = _aspect;
		}

		float GetRotationFactor() const
		{
			return m_RotationFactor;
		}
		float& GetRotationFactor()
		{
			return m_RotationFactor;
		}
		void SetRotationFactor(const float _rotationFactor)
		{
			m_RotationFactor = _rotationFactor;
		}

		float GetPitchRotationRate() const
		{
			return m_PitchRotationRate;
		}
		float& GetPitchRotationRate()
		{
			return m_PitchRotationRate;
		}
		void SetPitchRotationRate(const float _pitchRotationRate)
		{
			m_PitchRotationRate = _pitchRotationRate;
		}

		float GetYawRotationRate() const
		{
			return m_YawRotationRate;
		}
		float& GetYawRotationRate()
		{
			return m_YawRotationRate;
		}
		void SetYawRotationRate(const float _yawRotationRate)
		{
			m_YawRotationRate = _yawRotationRate;
		}

		glm::mat4 GetProjectionMatrix() const
		{
			return m_Projection;
		}
		glm::mat4& GetProjectionMatrix()
		{
			return m_Projection;
		}
		void SetProjectionMatrix(const glm::mat4& _projection)
		{
			m_Projection = _projection;
		}

		glm::mat4 GetViewMatrix() const
		{
			return m_View;
		}
		glm::mat4& GetViewMatrix()
		{
			return m_View;
		}
		void SetViewMatrix(const glm::mat4& _view)
		{
			m_View = _view;
		}

		glm::vec3 GetCameraFront() const
		{
			return m_CameraFront;
		}
		glm::vec3& GetCameraFront()
		{
			return m_CameraFront;
		}
		void SetCameraFront(const glm::vec3& _cameraFront)
		{
			m_CameraFront = _cameraFront;
		}

		glm::vec3 GetCameraUp() const
		{
			return m_CameraUp;
		}
		glm::vec3& GetCameraUp()
		{
			return m_CameraUp;
		}
		void SetCameraUp(const glm::vec3& _cameraUp)
		{
			m_CameraUp = _cameraUp;
		}

		// Ref<FrameBuffer> m_FrameBuffer;
	protected:
		bool m_IsPerspective = true;
		float m_Fov = 45.f;
		float m_NearPlane = 0.1f;
		float m_FarPlane = 10000.f;
		glm::vec2 m_PitchConstraints = glm::vec2(-89.f, 89.f);
		glm::vec2 m_Aspect = glm::vec2(800.f, 600.f);

		// Factor used to represent the speed of the rotation within respectable limits
		float m_RotationFactor = 100.0f;
		float m_PitchRotationRate = 1.0f;
		float m_YawRotationRate = 1.0f;


		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};

	class UICamera : public Camera
	{
	public:
		UICamera() : Camera({ "UI Camera" })
		{
			m_Layer = LayerType::UI;
            m_RenderComponent->SetIsVisible(false);
        }

		void Update(float _deltaTime) override
		{
            m_Projection = glm::ortho(0.0f, m_Aspect.x, 0.0f, m_Aspect.y, m_NearPlane, m_FarPlane);
		}
	};

	class ViewportCamera: public Camera
	{
	public:
		ViewportCamera(): Camera({"Viewport Camera"})
		{
			
		}

		float GetMoveSpeed() const
		{
			return m_MoveSpeed;
		}
		float& GetMoveSpeed()
		{
			return m_MoveSpeed;
		}
		void SetMoveSpeed(const float _moveSpeed)
		{
			m_MoveSpeed = _moveSpeed;
		}

		float GetMouseScrollSpeed() const
		{
            return m_MouseScrollSpeed;
        }
		float& GetMouseScrollSpeed()
		{
            return m_MouseScrollSpeed;
        }
		void Update(float _deltaTime) override;

	protected:
		float m_MoveSpeed = 10.0f;
		float m_MouseScrollSpeed = 0.5f;
	};
}
