#pragma once
#include "Denix/Scene/Scene.h"

namespace Denix
{
	class Character;
	class PipePair;

	class GraphicsGame final : public Scene
	{
	public:
		GraphicsGame() : Scene({ "Graphics Game" }) {}

		~GraphicsGame() override = default;

		bool Load() override;

		void BeginScene() override;

		void BeginPlay() override;

		void Update(float _deltaTime) override;
		void GameUpdate(float _deltaTime) override;

	private:
		void InitPipes();

		int PipePairCount = 0;

		float PipeMinSpeed = 0.8f;
		float PipeDistance = 5.0f;
		float PipeInitOffset = 2.0f;
		
		bool GameOver;

		/* Time to wait before accepting further user input*/
		float GameOverTime = 0.0f;
		float GameOverWaitTime = 2.0f;

		Ref<Character> Player;
	};
}