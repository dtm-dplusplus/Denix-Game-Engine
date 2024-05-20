#pragma once
#include "Denix/Scene/Scene.h"

namespace Denix
{
	class Character;

	class PlatformerScene final : public Scene
	{
	public:
		PlatformerScene() : Scene({ "Physics Scene" }) {}

		bool Load() override;
		void Update(float _deltaTime) override;

		Ref<Character> Player;

		Ref<GameObject> Sphere1;
		Ref<GameObject> Cube1;
	};
}
