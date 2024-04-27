#pragma once
#include "Denix/Scene/Scene.h"

using namespace Denix;

class Character : public GameObject
{
public:
	Character();

	void BeginPlay() override;
	void EndPlay() override;

	void BeginScene() override;
	void EndScene() override;

	void Update(float _deltaTime) override;

	float m_MoveSpeed = 1.5f;
	float m_SprintSpeed = 3.0f;
	float m_JumpForce = 50.0f;

	float m_CameraBoomLength = 10.0f;

	// Player  Camera
	Ref<Camera> m_FollowCamera;
};

class PlatformerScene final : public Scene
{
public:
	PlatformerScene(): Scene("Platformer"){}

	bool Load() override;

	void Update(float _deltaTime) override;

	void BeginPlay() override;
	Ref<Character> Player;
};

