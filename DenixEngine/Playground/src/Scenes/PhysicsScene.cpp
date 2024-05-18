#include "PhysicsScene.h"
#include "imgui.h"
#include "Denix/Core/TimerSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/Object/Shapes/Shapes.h"
#include "Denix/Video/Renderer/RendererSubsystem.h"
#include "Denix/Core/Math.h"

namespace Denix
{
	PhysObject::PhysObject(const ObjectInitializer& _objInit) : Cube(_objInit)
	{
		m_TransformComponent->SetMoveability(Moveability::Dynamic);
	}

	bool PhysicsScene::Load()
	{
		Scene::Load();

		Ref <Plane> floor = MakeRef<Plane>();
		//floor->GetTransformComponent()->SetScale(glm::vec3(5.0f, 1.0f, 5.0f));
		m_SceneObjects.push_back(floor);

		Cube1 = MakeRef<PhysObject>(ObjectInitializer("Cube 1"));
		Cube1->GetTransformComponent()->SetPosition({ 0.0f, 10.0f, 0.0f });
		Cube1->GetRenderComponent()->GetMaterial()->GetBaseParam().Color = { 0.0f, 1.0f, 1.0f };
		m_SceneObjects.push_back(Cube1);

		Cube2 = MakeRef<Cube>(ObjectInitializer("Cube 2"));
		Cube2->GetTransformComponent()->SetPosition({ 5.0f, 10.0f, 0.0f });
		Cube2->GetRenderComponent()->GetMaterial()->GetBaseParam().Color = { 0.0f, 0.5f, 1.0f };

		Ref<GameObject> Cone = MakeRef<GameObject>(ObjectInitializer("Cone"));
		ResourceSubsystem::LoadModel("SM_Cone", FileSubsystem::GetEngineContentRoot() + R"(models/Cone.fbx)");
		Cone->GetMeshComponent()->SetModel(ResourceSubsystem::GetModel("SM_Cone"));
		m_SceneObjects.push_back(Cone);

		m_DirLight->GetLightDirection().z = -0.54f;

		return true;
	}

	void PhysicsScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);
		
		ImGui::SetNextWindowPos(ImVec2(1000, 50));
		ImGui::Begin("Physics Scene Tools");
		ImGui::SliderFloat("Game Speed", &TimerSubsystem::GetGameTimeSpeed(), 0.0f, 2.0f);
		ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTimeMs());
		ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());
		ImGui::End();
	}
	
}