	#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Math/Math.h"
#include "Actor.h"
#include "Denix/Video/GL/Viewport.h"

namespace Denix
{
	class Camera : public Actor
	{
	public:
		/**
		 *  @brief Constructor for the Camera class
		 */
		Camera();
		
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

		Ref<Viewport> GetViewport() const
		{
			return m_Viewport;
		}

		bool m_ExternalControl = false;
		
		// Camera Properties
	public:
		bool m_IsGameCamera = false;
		bool m_IsPerspective = true;
		float m_Fov = 45.f;
		float m_NearPlane = 0.1f;
		float m_FarPlane = 10000.f;
		glm::vec2 m_PitchConstraints = glm::vec2(-89.f, 89.f);
		glm::vec2 m_Aspect = glm::vec2(800.f, 600.f);

		// Factor used to represent the speed of the rotation within respectable limits
		bool m_EnableRotation = true;
		float m_RotationFactor = 100.0f;
		float m_PitchRotationRate = 1.0f;
		float m_YawRotationRate = 1.0f;
		float m_MoveSpeed = 10.0f;
		float m_MouseScrollSpeed = 0.5f;
	
	private:
		// Viewport
		Ref<Viewport> m_Viewport;

		// Matrices
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		
		friend class Engine;
		friend class RendererSubsystem;
		friend class Scene;
		friend class SceneSubsystem;
	};
}
