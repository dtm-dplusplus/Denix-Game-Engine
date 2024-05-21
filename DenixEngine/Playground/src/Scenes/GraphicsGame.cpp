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

		/*Player = MakeRef<Character>();
		Player->GetTransformComponent()->SetPosition({ 0.0f, 5.0f, 5.0f });
		m_SceneObjects.push_back(Player);
		m_SceneObjects.push_back(Player->m_FollowCamera);*/

		Ref<PipePair> PipePair1 = MakeRef<PipePair>(PipePairCount++, ObjectInitializer("PipePair 1"));
		m_SceneObjects.push_back(PipePair1);
		m_SceneObjects.push_back(PipePair1->PipeTop);
		m_SceneObjects.push_back(PipePair1->PipeBottom);

		/*Ref<Cube> cube1 = MakeRef<Cube>(ObjectInitializer("Cube 1"));
		m_SceneObjects.push_back(cube1);

		Ref<GameObject> pipe = MakeRef<GameObject>(ObjectInitializer("Pipe"));
		pipe->GetMeshComponent()->SetModel(ResourceSubsystem::GetModel("SM_Pipe"));
		m_SceneObjects.push_back(pipe);*/

		m_DirLight->GetLightDirection().z = -0.54f;

		return true;
	}

	void GraphicsGame::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::SetNextWindowPos(ImVec2(1000, 50));
		ImGui::Begin("Physics Scene Tools");
		ImGui::SliderFloat("Game Speed", &TimerSubsystem::GetGameTimeSpeed(), 0.0f, 2.0f);
		ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTimeMs());
		ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());

		//ImGui::DragFloat("Jump Force", &Player->m_JumpForce, 0.1f);
		//ImGui::DragFloat("Move Speed", &Player->m_MoveSpeed, 0.1f);
		//
		//ImGui::SeparatorText("Character Camera");
		//ImGui::DragFloat("Camera Boom Length", &Player->m_CameraBoomLength, 0.1f);
		//ImGui::DragFloat("Camera Boom Height", &Player->m_CameraBoomHeight, 0.1f);


		ImGui::End();
	}

}