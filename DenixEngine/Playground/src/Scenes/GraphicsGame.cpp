#include "GraphicsGame.h"

#include "imgui.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Input/InputSubsystem.h"

#include "Denix/Scene/Object/Shapes/Shapes.h"
#include "Denix/Core/Math.h"
#include "Graphics/JumpPawn.h"
#include "Graphics/Pipe.h"

namespace Denix
{

	void GraphicsGame::InitPipes()
	{
		Ref<Model> pipeModel = ResourceSubsystem::GetModel("SM_Pipe");
		for (int i = 0; i < 100; i++)
		{
			Ref<PipePair> pipePair = MakeRef<PipePair>(PipePairCount, ObjectInitializer("PipePair " + std::to_string(PipePairCount)));
			pipePair->GetTransformComponent()->SetPosition({ PipeInitOffset + i * PipePairDistance, 0.0f, 0.0f });
			pipePair->InitPipePair();
			pipePair->PipeTop->GetMeshComponent()->SetModel(pipeModel);
			pipePair->PipeBottom->GetMeshComponent()->SetModel(pipeModel);

			BaseMatParam& baseTop = pipePair->PipeTop->GetRenderComponent()->GetMaterial()->GetBaseParam();
			baseTop.Texture = ResourceSubsystem::GetTexture("Brick");
			baseTop.IsTexture = true;

			BaseMatParam& baseBottom = pipePair->PipeBottom->GetRenderComponent()->GetMaterial()->GetBaseParam();
			baseBottom.Texture = ResourceSubsystem::GetTexture("Brick");
			baseBottom.IsTexture = true;

			m_SceneObjects.push_back(pipePair);
			m_SceneObjects.push_back(pipePair->PipeTop);
			m_SceneObjects.push_back(pipePair->PipeBottom);
			PipePairCount++;
		}
	}

	bool GraphicsGame::Load()
	{
		Scene::Load();

		GameStartPlane = MakeRef<Plane>(ObjectInitializer("GameStartPlane"));
		BaseMatParam& baseGameStart = GameStartPlane->GetRenderComponent()->GetMaterial()->GetBaseParam();
		GameStartPlane->GetRenderComponent()->SetIsVisible(true);
		GameStartPlane->GetRenderComponent()->SetAffectsLighting(false);
		GameStartPlane->GetPhysicsComponent()->CollisionDetectionEnabled() = false;
		GameStartPlane->GetTransformComponent()->SetPosition({ 4.0f, 0.0f, 6.0f });
		GameStartPlane->GetTransformComponent()->GetRotation().x = 90.0f;
		baseGameStart.Texture = ResourceSubsystem::GetTexture("GameStart");
		baseGameStart.IsTexture = true;
		m_SceneObjects.push_back(GameStartPlane);

		GameOverPlane = MakeRef<Plane>(ObjectInitializer("GameOverPlane"));
		GameOverPlane->GetTransformComponent()->SetPosition({ 4.0f, 0.0f, 6.0f });
		GameOverPlane->GetTransformComponent()->GetRotation().x = 90.0f;
		GameOverPlane->GetRenderComponent()->SetIsVisible(false);
		GameOverPlane->GetRenderComponent()->SetAffectsLighting(false);
		GameOverPlane->GetPhysicsComponent()->CollisionDetectionEnabled() = false;
		BaseMatParam& baseGameOver = GameOverPlane->GetRenderComponent()->GetMaterial()->GetBaseParam();
		baseGameOver.Texture = ResourceSubsystem::GetTexture("GameEnd");
		baseGameOver.IsTexture = true;
		m_SceneObjects.push_back(GameOverPlane);
		
		Ref<Plane> background = MakeRef<Plane>(ObjectInitializer("Background"));
		background->GetRenderComponent()->GetMaterial()->GetBaseParam().Color = {0.0f,188.0f/255.0f, 1.0f};
		background->GetTransformComponent()->SetScale({ 100.0f, 100.0f, 100.0f });
		background->GetTransformComponent()->GetPosition().z = -50.0f;
		background->GetTransformComponent()->GetRotation().x = 90.0f;
		background->GetPhysicsComponent()->CollisionDetectionEnabled() = false;
		m_SceneObjects.push_back(background);

	/*	Ref<PointLight> pointLight1 = MakeRef<PointLight>(ObjectInitializer("PointLight 1"));
		pointLight1->GetRenderComponent()->IsVisible() = false;
		pointLight1->GetLightColor() = { 188.0f / 255.0f, 1.0f, 1.0f };
		pointLight1->GetTransformComponent()->SetPosition({ 2.0f, 2.0f, 0.0f });
		m_SceneObjects.push_back(pointLight1);*/

		Player = MakeRef<JumpPawn>();
		Player->GetMeshComponent()->SetModel(ResourceSubsystem::GetModel("SM_Bird"));
		BaseMatParam& basePlayer = Player->GetRenderComponent()->GetMaterial()->GetBaseParam();
		basePlayer.IsTexture = true;
		basePlayer.Texture = ResourceSubsystem::GetTexture("Brick");

		m_SceneObjects.push_back(Player);
		m_SceneObjects.push_back(Player->FollowCamera);

		InitPipes();

		m_DirLight->GetLightDirection().z = -0.54f;

		return true;
	}

	void GraphicsGame::BeginScene()
	{
		Scene::BeginScene();

		// Set initial viewport cam
		glm::vec3& campos = m_ActiveCamera->GetTransformComponent()->GetPosition();
		const glm::vec3 camOffset = Player->CameraOffset;
		campos = Player->FollowCamera->GetTransformComponent()->GetPosition();
		campos = { camOffset.x, camOffset.y,camOffset.z };
		m_ActiveCamera->GetTransformComponent()->SetRotation({ 0.0f, -90.0f, 0.0f });
		m_ActiveCamera->GetTransformComponent()->GetPosition().y = 3.0f;
	}

	void GraphicsGame::BeginPlay()
	{
		Scene::BeginPlay();

		GameOver = false;
		GameOverTime = 0.0f;
		GameStart = false;
	}

	void GraphicsGame::GameUpdate(float _deltaTime)
	{
		Scene::GameUpdate(_deltaTime);

		if (Player)
		{
			if (Player->HitPipe)
			{
				GameOver = true;
			}

			if (InputSubsystem::IsKeyDown(SDL_SCANCODE_SPACE))
			{
				if (!GameStart)
				{
					GameStart = true;
					GameStartPlane->GetRenderComponent()->SetIsVisible(false);
					Player->GetPhysicsComponent()->SetSimulatePhysics(true);
					PipePair::CanMove = true;
				}

				if (GameStart && !GameOver) Player->GetPhysicsComponent()->AddImpulse({0.0f, Player->JumpForce * _deltaTime, 0.0f});
				
				// Player hits space after game over to play again
				if (GameOver && GameOverTime >= GameOverWaitTime) m_RequestStop = true;
			}
		}

		PipePair::MoveSpeed += _deltaTime * 0.1f;

		if (GameOver)
		{
			GameOverTime += _deltaTime;

			GameOverPlane->GetRenderComponent()->SetIsVisible(true);
			PipePair::CanMove = false;
		}
	}
}