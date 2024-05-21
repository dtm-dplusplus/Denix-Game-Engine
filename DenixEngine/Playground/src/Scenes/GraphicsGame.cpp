#include "GraphicsGame.h"

#include "imgui.h"
#include "Denix/Core/TimerSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/Object/Shapes/Shapes.h"
#include "Denix/Core/Math.h"
#include "Graphics/Character.h"
#include "Graphics/Pipe.h"

namespace Denix
{
	bool GraphicsGame::Load()
	{
		Scene::Load();

		Player = MakeRef<Character>();
		Player->GetTransformComponent()->SetPosition({ -10.0f, 5.0f, 5.0f });
		m_SceneObjects.push_back(Player);
		//m_SceneObjects.push_back(Player->FollowCamera);

		for (int i = 0; i < 10; i++)
		{
			Ref<PipePair> pipePair = MakeRef<PipePair>(PipePairCount, ObjectInitializer("PipePair " + std::to_string(PipePairCount)));
			pipePair->GetTransformComponent()->SetPosition({ i * PipeDistance, 0.0f, 0.0f });
			pipePair->InitPipePair();
			m_SceneObjects.push_back(pipePair);
			m_SceneObjects.push_back(pipePair->PipeTop);
			m_SceneObjects.push_back(pipePair->PipeBottom);

			PipePairs.push_back(pipePair);
			PipePairCount++;
        }

		m_DirLight->GetLightDirection().z = -0.54f;

		return true;
	}

	void GraphicsGame::BeginPlay()
	{
		Scene::BeginPlay();

		GameOver = false;
	}

	void GraphicsGame::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::SetNextWindowPos(ImVec2(1000, 50), ImGuiCond_Appearing);

		ImGui::Begin("Graphics Game");
		ImGui::DragFloat("Pipe Distance", &PipeDistance, 0.1f);
		ImGui::DragFloat("Pipe Speed", &PipePair::MoveSpeed, 0.1f);
		ImGui::SeparatorText("Character Camera");
		if (Player)
		{
			ImGui::DragFloat("Jump Force", &Player->JumpForce, 0.1f);

			if (Player->FollowCamera)
			{
				ImGui::DragFloat("Camera Boom Length", &Player->CameraBoomLength, 0.1f);
				ImGui::DragFloat("Camera Boom Height", &Player->CameraBoomHeight, 0.1f);
			}
		}

		
		
		/*ImGui::Checkbox("Game Over", &GameOver);
		if (ImGui::Button("Restart Game")) m_RequestRe

		if (Player->HitPipe)
		{
			 GameOver = true;
        }

		if (GameOver)
		{
            
        }*/
		//
		//
		//


		ImGui::End();
	}

}