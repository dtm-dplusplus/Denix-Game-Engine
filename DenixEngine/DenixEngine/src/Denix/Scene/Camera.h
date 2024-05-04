#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GameObject.h"
#include "Component/TransformComponent.h"
#include "Denix/Video/Renderer/RenderComponent.h"

namespace Denix
{
	struct RenderTexture
		{
			RenderTexture(int _width, int _height)
			{
				glGenFramebuffers(1, &m_fboId);
				if (!m_fboId) throw std::exception();
				glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);

				glGenTextures(1, &m_texId);
				glBindTexture(GL_TEXTURE_2D, m_texId);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glBindTexture(GL_TEXTURE_2D, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texId, 0);

				glGenRenderbuffers(1, &m_rboId);
				glBindRenderbuffer(GL_RENDERBUFFER, m_rboId);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rboId);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			~RenderTexture() {}

			void bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_fboId); }
			void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
			GLuint getTexture() { return m_texId; }

		private:
			GLuint m_fboId;
			GLuint m_texId;
			GLuint m_rboId;
		};
	class Camera : public GameObject
	{
	public:
		Camera(const ObjectInitializer& _objInit = {"Camera"}) : GameObject(ObjectInitializer(_objInit))
		{
			m_TransformComponent->SetPosition(glm::vec3(0.0f, 0.0f, 5));
			m_TransformComponent->SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
			m_RenderComponent->SetIsVisible(false);

			m_RenderTexture = MakeRef<RenderTexture>(1368, 768);
		}

		~Camera() override = default;

		

		Ref<RenderTexture> m_RenderTexture;

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

		void Update(float _deltaTime) override;

	protected:
		float m_MoveSpeed = 100.0f;
	};
}
