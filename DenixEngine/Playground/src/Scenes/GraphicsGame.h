#pragma once
#include "Denix/Scene/Scene.h"

namespace Denix
{
	class Character;
	class PipePair;

	class GraphicsGame final : public Scene
	{
	public:
		GraphicsGame() : Scene({ "Physics Scene" }) {}

		bool Load() override;

		void BeginPlay() override;

		void Update(float _deltaTime) override;

		int PipePairCount = 0;

		float PipeDistance = 5.0f;

		bool GameOver;

		Ref<Character> Player;

		std::vector<Ref<PipePair>> PipePairs;
	};
}