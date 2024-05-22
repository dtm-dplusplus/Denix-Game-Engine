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
		for (int i = 0; i < 10; i++)
		{
			Ref<PipePair> pipePair = MakeRef<PipePair>(PipePairCount, ObjectInitializer("PipePair " + std::to_string(PipePairCount)));
			pipePair->GetTransformComponent()->SetPosition({ PipeInitOffset + i * PipeDistance, 0.0f, 0.0f });
			pipePair->InitPipePair();
			pipePair->PipeTop->GetMeshComponent()->SetModel(pipeModel);
			pipePair->PipeBottom->GetMeshComponent()->SetModel(pipeModel);
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
		baseGameStart.Texture = ResourceSubsystem::GetTexture("GameStart");
		baseGameStart.IsTexture = true;
		m_SceneObjects.push_back(GameStartPlane);

		GameOverPlane = MakeRef<Plane>(ObjectInitializer("GameOverPlane"));
		BaseMatParam& baseGameOver = GameOverPlane->GetRenderComponent()->GetMaterial()->GetBaseParam();
		baseGameOver.Texture = ResourceSubsystem::GetTexture("GameEnd");
		baseGameOver.IsTexture = true;
		m_SceneObjects.push_back(GameOverPlane);
		
		Player = MakeRef<JumpPawn>();
		Player->GetMeshComponent()->SetModel(ResourceSubsystem::GetModel("SM_Bird"));
		

		m_SceneObjects.push_back(Player);
		m_SceneObjects.push_back(Player->FollowCamera);

		InitPipes();

		m_DirLight->GetLightDirection().z = -0.54f;

		
		m_UICamera->m_Texts.push_back({ "Score: 0", { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f });
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
	}

	void GraphicsGame::BeginPlay()
	{
		Scene::BeginPlay();

		GameOver = false;
		GameOverTime = 0.0f;
		GameStart = false;
	}

	void GraphicsGame::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::SetNextWindowPos(ImVec2(1000, 50), ImGuiCond_Appearing);

		ImGui::Begin("Graphics Game");
		ImGui::Checkbox("Game Over", &GameOver);
		if (ImGui::Button("Force Restart Game"))
		{
			m_RequestStop = true;
			//return;
		}
		if (GameOver)
		{
			if (ImGui::Button("Play Again"))
			{
                m_RequestStop = true;
                //return;
            }
		}
		ImGui::DragFloat("Pipe Distance", &PipeDistance, 0.1f);
		ImGui::DragFloat("Pipe Speed", &PipePair::MoveSpeed, 0.1f);
		ImGui::DragFloat("Pipe Destruction Distance", &PipePair::DestructionDistance, 0.1f);
		ImGui::SeparatorText("Character Camera");
		ImGui::DragFloat("Jump Force", &Player->JumpForce, 0.1f);
		ImGui::DragFloat3("Camera Offset", &Player->CameraOffset[0], 0.1f);
		ImGui::End();
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
					Player->GetPhysicsComponent()->SetSimulatePhysics(true);
					PipePair::CanMove = true;
				}

				// Player hits space after game over to play again
				if (GameOver && GameOverTime >= GameOverWaitTime) m_RequestStop = true;
			}
		}

		if (GameOver)
		{
			GameOverTime += _deltaTime;

			PipePair::CanMove = false;
		}
	}
}