#pragma once
#include "Denix/Scene/Scene.h"

namespace Denix
{
	class JumpPawn;
	class PipePair;

	class GraphicsGame final : public Scene
	{
	public:
		GraphicsGame() : Scene({ "Graphics Game" }) {}

		~GraphicsGame() override = default;

		bool Load() override;

		void BeginScene() override;

		void BeginPlay() override;

		void GameUpdate(float _deltaTime) override;

	private:
		void InitPipes();

		int PipePairCount = 0;

		float PipeSpeed = 1.5f;
		float PipePairDistance = 8.0f;
		float PipeInitOffset = 10.0f;
		
		bool GameStart;
		bool GameOver;

		/* Time to wait before accepting further user input*/
		float GameOverTime = 0.0f;
		float GameOverWaitTime = 0.5f;

		Ref<GameObject> GameStartPlane;
		Ref<GameObject> GameOverPlane;

		Ref<JumpPawn> Player;
	};
}