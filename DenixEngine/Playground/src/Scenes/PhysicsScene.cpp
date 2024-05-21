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
		//floor->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		floor->GetPhysicsComponent()->CollisionDetectionEnabled() = true;
		floor->GetPhysicsComponent()->IsColliderVisible() = true;
		floor->GetTransformComponent()->SetScale(glm::vec3(30.0f, 0.01f, 30.0f));
		m_SceneObjects.push_back(floor);

		Sphere1 = MakeRef<Sphere>(ObjectInitializer("Sphere 1"));
		Sphere1->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		Sphere1->GetPhysicsComponent()->IsColliderVisible() = true;
		Sphere1->GetPhysicsComponent()->SetSimulatePhysics(true);
		Sphere1->GetPhysicsComponent()->GetLinearDrag() = 1.0f;
		Sphere1->GetTransformComponent()->SetPosition({ 0.0f, 5.0f, 0.0f });
		Sphere1->GetRenderComponent()->GetMaterial()->GetBaseParam().Color = { 0.0f, 1.0f, 1.0f };
		m_SceneObjects.push_back(Sphere1);

		//Cube1 = MakeRef<Cube>(ObjectInitializer("Cube 1"));
		//Cube1->GetPhysicsComponent()->IsColliderVisible() = true;
		//Cube1->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		//Cube1->GetPhysicsComponent()->SetSimulatePhysics(false);
		//Cube1->GetTransformComponent()->SetPosition({ 3.0f, 5.0f, 0.0f }); //
		//Cube1->GetRenderComponent()->GetMaterial()->GetBaseParam().Color = { 0.0f, 0.5f, 0.0f };
		//m_SceneObjects.push_back(Cube1);

		//Ref<Cube> cube2 = MakeRef<Cube>(ObjectInitializer("Cube 2"));
		//cube2->GetPhysicsComponent()->IsColliderVisible() = true;
		//cube2->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		//cube2->GetPhysicsComponent()->SetSimulatePhysics(false);
		//cube2->GetTransformComponent()->SetPosition(glm::vec3(7.0f, 5.0f, 0.0f));
		//cube2->GetRenderComponent()->GetMaterial()->GetBaseParam().Color = { 1.0f, 0.5f, 0.0f };
		//m_SceneObjects.push_back(cube2);

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
		
		static glm::vec3 force= { 0.0f, 15.0f, 0.0f };
		static glm::vec3 torque = { 0.0f, 1.0f, 0.0f };
		ImGui::DragFloat3("Force", &force.x, 0.1f);
		ImGui::DragFloat3("Torque", &torque.x, 0.1f);
		if (ImGui::Button("Add Force to Sphere 1")) Sphere1->GetPhysicsComponent()->AddImpulse(force);
		if (ImGui::Button("Add Torque to Sphere 1")) Sphere1->GetPhysicsComponent()->AddTorque(torque);
		ImGui::End();
	}
	
}