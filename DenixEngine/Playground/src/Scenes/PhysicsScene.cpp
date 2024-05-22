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
		floor->GetPhysicsComponent()->IsColliderVisible() = false;
		floor->GetTransformComponent()->SetScale(glm::vec3(30.0f, 0.01f, 30.0f));
		m_SceneObjects.push_back(floor);

		Sphere1 = MakeRef<Sphere>(ObjectInitializer("Sphere 1"));
		Sphere1->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		Sphere1->GetPhysicsComponent()->IsColliderVisible() = false;
		Sphere1->GetPhysicsComponent()->SetSimulatePhysics(true);
		Sphere1->GetPhysicsComponent()->GetLinearDrag() = 1.0f;
		Sphere1->GetTransformComponent()->SetPosition({ -2.0f, 3.0f, 0.0f });
		Sphere1->GetRenderComponent()->GetMaterial()->GetBaseParam().Color = { 0.0f, 1.0f, 1.0f };
		m_SceneObjects.push_back(Sphere1);

	/*	Sphere2 = MakeRef<Sphere>(ObjectInitializer("Sphere 2"));
		Sphere2->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		Sphere2->GetPhysicsComponent()->IsColliderVisible() = true;
		Sphere2->GetPhysicsComponent()->SetSimulatePhysics(true);
		Sphere2->GetPhysicsComponent()->GetLinearDrag() = 1.0f;
		Sphere2->GetTransformComponent()->SetPosition({ 2.0f, 3.0f, 0.0f });
		m_SceneObjects.push_back(Sphere2);*/

		//Cube1 = MakeRef<Cube>(ObjectInitializer("Cube 1"));
		//Cube1->GetPhysicsComponent()->IsColliderVisible() = true;
		//Cube1->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		//Cube1->GetPhysicsComponent()->SetSimulatePhysics(false);
		//Cube1->GetTransformComponent()->SetPosition({ 3.0f, 5.0f, 0.0f }); //
		//Cube1->GetRenderComponent()->GetMaterial()->GetBaseParam().Color = { 0.0f, 0.5f, 0.0f };
		//m_SceneObjects.push_back(Cube1);

		Ref<Cube> cube2 = MakeRef<Cube>(ObjectInitializer("Cube 2"));
		cube2->GetPhysicsComponent()->IsColliderVisible() = true;
		cube2->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		cube2->GetPhysicsComponent()->SetSimulatePhysics(false);
		cube2->GetTransformComponent()->SetPosition(glm::vec3(7.0f, 5.0f, 0.0f));
		cube2->GetRenderComponent()->GetMaterial()->GetBaseParam().Color = { 1.0f, 0.5f, 0.0f };
		m_SceneObjects.push_back(cube2);

		

		return true;
	}
	void PhysicsScene::BeginScene()
	{
		Scene::BeginScene();

		m_ActiveCamera->GetTransformComponent()->SetPosition({ -3.0f, 3.0f, 35.0f });
		m_ActiveCamera->GetTransformComponent()->GetRotation().y = -70.0f;
		m_DirLight->GetLightDirection().z = -0.54f;
	}
	void PhysicsScene::BeginPlay()
	{
		Scene::BeginPlay();

		Sphere1->GetPhysicsComponent()->AddImpulse(StartImpulse);
		////Sphere2->GetPhysicsComponent()->AddImpulse(-StartImpulse);
	}

	void PhysicsScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);
		
		ImGui::SetNextWindowPos(ImVec2(1000, 50));
		ImGui::Begin("Physics Scene Tools");
		ImGui::DragFloat3("Start Impulse", &StartImpulse.x, 0.1f);
		
		static glm::vec3 force= { 0.0f, 15.0f, 0.0f };
		static glm::vec3 torque = { 0.0f, 1.0f, 0.0f };
		ImGui::DragFloat3("Force", &force.x, 0.1f);
		ImGui::DragFloat3("Torque", &torque.x, 0.1f);
		if (ImGui::Button("Add Force to Sphere 1")) Sphere1->GetPhysicsComponent()->AddImpulse(force);
		if (ImGui::Button("Add Torque to Sphere 1")) Sphere1->GetPhysicsComponent()->AddTorque(torque);
		ImGui::End();
	}
	
}