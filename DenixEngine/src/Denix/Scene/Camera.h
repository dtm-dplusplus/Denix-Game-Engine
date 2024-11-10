	#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Math.h"
#include "GameObject.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Video/GL/Viewport.h"

namespace Denix
{
	class Camera : public GameObject
	{
	public:
		/**
		 *  @brief Constructor for the Camera class
		 * @param _position Option to set the position of the camera 
		 * @param _objInit ObjectInitializer for the GameObject 
		 */
		Camera(const ObjectInit& _objInit = {"Camera"}, const glm::vec3 _position = {0.0f, 5.0f, 25.0f}) : GameObject(ObjectInit(_objInit))
		{
			m_TransformComponent->SetPosition(_position);
			m_TransformComponent->SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
			m_RenderComponent->SetIsVisible(false);
			m_Viewport = MakeRef<Viewport>(GetName() + "_Viewport");
		}
		
		~Camera() override = default;

		void ProcessKeyboardInput(float _deltaTime);
		void ProccessMouseMovement(float _deltaTime);
		void Update(float _deltaTime) override;


		glm::mat4 GetProjectionMatrix() const
		{
			return m_Projection;
		}
		glm::mat4& GetProjectionMatrix()
		{
			return m_Projection;
		}

		glm::mat4 GetViewMatrix() const
		{
			return m_View;
		}
		glm::mat4& GetViewMatrix()
		{
			return m_View;
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

		// Camera Properties
	public:
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
		float m_MoveSpeed = 10.0f;
		float m_MouseScrollSpeed = 0.5f;
	
		glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_CameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
		
	private:
		// Viewport
		Ref<Viewport> m_Viewport;

		// Matrices
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);
		
		friend class Engine;
		friend class Scene;
	};
}
