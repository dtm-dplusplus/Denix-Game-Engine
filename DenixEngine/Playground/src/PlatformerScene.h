#pragma once
#include "Denix/Scene/Scene.h"

using namespace Denix;

class Character : public GameObject
{
public:
	Character(){}

	void BeginPlay() override;
	void EndPlay() override;

	void BeginScene() override;
	void EndScene() override;

	void Update(float _deltaTime) override;


	// Player  Camera
	Ref<Camera> m_FPCamera;
};

class PlatformerScene final : public Denix::Scene
{
public:
	PlatformerScene(): Scene("Platformer"){}

	bool Load() override;

	void Update(float _deltaTime) override;

	Ref<Character> Player;
};

