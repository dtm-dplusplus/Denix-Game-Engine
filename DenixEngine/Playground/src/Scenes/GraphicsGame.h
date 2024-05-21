#pragma once
#include "Denix/Scene/Scene.h"

namespace Denix
{
	class Character;

	class GraphicsGame final : public Scene
	{
	public:
		GraphicsGame() : Scene({ "Physics Scene" }) {}

		bool Load() override;
		void Update(float _deltaTime) override;

		int PipePairCount = 0;

		Ref<Character> Player;

		Ref<GameObject> Sphere1;
		Ref<GameObject> Cube1;
	};
}
