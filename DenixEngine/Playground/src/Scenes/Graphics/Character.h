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

		void GameUpdate(float _deltaTime) override;

		void OnCollision(Ref<GameObject>& _other, CollisionData& _collision) override;

		void BeginPlay() override;

		bool HitPipe;

		float JumpForce = 0.1f;
		
		glm::vec3 CameraOffset = { 2.0f, 0.0f, 10.0f };

		int Score = 0;

		// Character  Camera
		Ref<Camera> FollowCamera;
	};
}