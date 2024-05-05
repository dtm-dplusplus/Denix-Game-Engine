#include "PhysicsScene.h"
#include "imgui.h"
#include <Denix/Core/TimerSubsystem.h>

#include "Denix/Scene/Object/Shapes/Shapes.h"

namespace Denix
{
	bool PhysicsScene::Load()
	{
		Scene::Load();

		Ref <Plane> floor = MakeRef<Plane>();
		floor->GetTransformComponent()->SetRotation({90.0f, 0.0f,0.0f});
		floor->GetTransformComponent()->SetScale(glm::vec3(50.0f, 50.0f, 1.0f));
		m_SceneObjects.push_back(floor);

		PntLight = MakeRef<PointLight>();
		PntLight->GetTransformComponent()->SetPosition({ 0.0f, 7.0f, 0.0f });
		PntLight->SetLightColor({ 1.0f,0.0f,0.0f });
		m_SceneObjects.push_back(PntLight);

		Ref<Cube> red = MakeRef<Cube>();
		red->GetTransformComponent()->SetPosition({ 5.0f, 10.0f, 0.0f });
		red->GetRenderComponent()->SetMaterial(ResourceSubsystem::GetMaterial("MAT_Red"));

		SptLight = MakeRef<SpotLight>();
		SptLight->GetTransformComponent()->SetPosition({ -5.0f, 5.0f, 0.0f });
		m_SceneObjects.push_back(SptLight);

		PhysicsObject = MakeRef<PhysObject>();
		PhysicsObject->GetTransformComponent()->SetPosition({ 0.0f, 10.0f, 0.0f });
		PhysicsObject->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		PhysicsObject->GetPhysicsComponent()->SimulatePhysics() = true;

		PhysicsObject->GetMeshComponent()->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));
		m_SceneObjects.push_back(PhysicsObject);

		m_DirLight->SetAmbientIntensity(0.7f);
		m_ViewportCamera->SetFov(100.0f);
		m_ViewportCamera->GetTransformComponent()->SetPosition({ 0.0f,5.0f, 10.0f });

		return true;
	}

	void PhysicsScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);
		
		ImGui::SetNextWindowPos(ImVec2(1000, 50));
		ImGui::Begin("Physics Scene Tools");
		ImGui::SliderFloat("Game Speed", &TimerSubsystem::GetGameTimeSpeed(), 0.0f, 2.0f);
		ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTime());
		ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());
		ImGui::End();
	}
}