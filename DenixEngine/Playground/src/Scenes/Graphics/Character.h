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

		float JumpForce = 1.0f;
		
		// Character  Camera
		Ref<Camera> FollowCamera;

		float CameraBoomLength = 1.0f;
		float CameraBoomHeight = 1.0f;
	};
}