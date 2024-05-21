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

		void OnCollision(Ref<GameObject>& _other, CollisionData& _collision) override;

		void BeginPlay() override;

		bool HitPipe;

		float MoveSpeed = 5.0f;
		float JumpForce = 1.0f;
		
		// Character  Camera
		Ref<Camera> m_FollowCamera;

		float m_CameraBoomLength = 1.0f;
		float m_CameraBoomHeight = 1.0f;
	};
}