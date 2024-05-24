#pragma once

#include "Denix/Core.h"
#include "Denix/Scene/GameObject.h"

namespace Denix
{
	class Camera;

	class JumpPawn : public GameObject
	{
	public:
		JumpPawn();
		~JumpPawn() = default;

		void OnCollision(Ref<GameObject>& _other, CollisionData& _collision) override;

		void BeginPlay() override;

		bool HitPipe;

		float JumpForce = 100.0f;
		
		glm::vec3 CameraOffset = { 4.0f, 0.0f, 10.0f };

		int Score = 0;

		// JumpPawn  Camera
		Ref<Camera> FollowCamera;
	};
}