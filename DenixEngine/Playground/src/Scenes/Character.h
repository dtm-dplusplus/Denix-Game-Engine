#pragma once

#include "Denix/Core.h"
#include "Denix/Scene/GameObject.h"

namespace Denix
{
	class Camera;

	class Character : public GameObject
	{
	public:
		Character();
		~Character() = default;

		void Update(float _deltaTime) override;

		float m_MoveSpeed = 1.5f;
		float m_SprintSpeed = 3.0f;
		float m_JumpForce = 50.0f;

		float m_CameraBoomLength = 10.0f;

		// Character  Camera
		Ref<Camera> m_FollowCamera;
	};


}
