#include "PlatformerScene.h"

#include "imgui.h"
#include "Denix/Core/TimerSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/Object/Shapes/Shapes.h"
#include "Denix/Core/Math.h"
#include "Character.h"

namespace Denix
{
	bool PlatformerScene::Load()
	{
		Scene::Load();

		Ref <Plane> floor = MakeRef<Plane>();
		floor->GetPhysicsComponent()->CollisionDetectionEnabled() = true;
		floor->GetPhysicsComponent()->IsColliderVisible() = true;
		floor->GetTransformComponent()->SetScale(glm::vec3(30.0f, 0.01f, 30.0f));
		m_SceneObjects.push_back(floor);

		Player = MakeRef<Character>();
		Player->GetTransformComponent()->SetPosition({ 0.0f, 5.0f, 5.0f });
		m_SceneObjects.push_back(Player);
		m_SceneObjects.push_back(Player->m_FollowCamera);

		m_DirLight->GetLightDirection().z = -0.54f;

		return true;
	}

	void PlatformerScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::SetNextWindowPos(ImVec2(1000, 50));
		ImGui::Begin("Physics Scene Tools");
		ImGui::SliderFloat("Game Speed", &TimerSubsystem::GetGameTimeSpeed(), 0.0f, 2.0f);
		ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTimeMs());
		ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());
		static glm::vec3 force = { 0.0f, 15.0f, 0.0f };
		ImGui::DragFloat("Jump Force", &force.y, 0.1f);
		ImGui::DragFloat("Move Speed", &Player->m_MoveSpeed, 0.1f);

		ImGui::DragFloat3("Force", &force.x, 0.1f);
		if (ImGui::Button("Add Force to Sphere 1")) Sphere1->GetPhysicsComponent()->AddImpulse(force);
		ImGui::End();
	}

}