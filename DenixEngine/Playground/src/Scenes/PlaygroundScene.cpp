#include "PlaygroundScene.h"

#include "imgui.h"

namespace Denix
{
	bool PlaygroundScene::Load()
	{
		Scene::Load();

		//m_Camera->GetTransformComponent()->SetPosition({ -20.0, 15.0, 55.0 });
		//m_Camera->GetTransformComponent()->SetRotation({ -5.0, -70.0, 0.0 });
		//m_Camera->GetFov() = 80.0f;

		//Ref<Cube> cube = MakeRef<Cube>();
		//cube->SetMoveability(Moveability::Dynamic);
		//Ref<TransformComponent> transform = cube->GetTransformComponent();
		//transform->SetPosition({ 15.0, 50,0.0 });
		//transform->SetRotation({ 0.0, 0.0, 0.0 });
		//cube->GetRenderComponent()->SetDebugColor({ 1.0, 80.0/255.0, 50.0/255.0, 1.0 });
		//m_SceneObjects.push_back(cube);
		//
		//// Walls
		//Ref<Plane> wall1 = MakeRef<Plane>();
		//Ref<TransformComponent> wall1Transform = wall1->GetTransformComponent();
		//wall1Transform->SetPosition({ 0.0, 15.0, 0.0 });
		//wall1Transform->SetRotation({ 0.0, 90.0, 0.0 });
		//wall1Transform->SetScale({ 30.0, 30.0, 1.0 });
		//wall1->GetRenderComponent()->SetDebugColor({ 0.0, 1.0, 1.0, 1.0 });
		//wall1->GetPhysicsComponent()->IsSimulated() = false;
		//m_SceneObjects.push_back(wall1);
		//
		//Ref<Plane> wall2 = MakeRef<Plane>();
		//Ref<TransformComponent> wall2Transform = wall2->GetTransformComponent();
		//wall2Transform->SetPosition({ 15.0, 0.0, 0.0 });
		//wall2Transform->SetRotation({ 90.0,0.0, 0.0 });
		//wall2Transform->SetScale({ 30.0, 30.0, 1.0 });
		//wall2->GetRenderComponent()->SetDebugColor({ 1.0, 0.0, 1.0, 1.0 });
		//wall2->GetPhysicsComponent()->IsSimulated() = false;
		//m_SceneObjects.push_back(wall2);
		//
		//Ref<Plane> wall3 = MakeRef<Plane>();
		//Ref<TransformComponent> wall3Transform = wall3->GetTransformComponent();
		//wall3Transform->SetPosition({ 30.0, 15.0, 0.0 });
		//wall3Transform->SetRotation({ 0.0, 90.0, 0.0 });
		//wall3Transform->SetScale({ 30.0, 30.0, 1.0 });
		//wall3->GetRenderComponent()->SetDebugColor({ 45.0 / 255.0, 1.0, 0.0, 1.0 });
		//wall3->GetPhysicsComponent()->IsSimulated() = false;
		//m_SceneObjects.push_back(wall3);
		//
		//Ref<Plane> wall4 = MakeRef<Plane>();
		//Ref<TransformComponent> wall4Transform = wall4->GetTransformComponent();
		//wall4Transform->SetPosition({ 15.0, 15.0, -15.0 });
		//wall4Transform->SetRotation({ 0.0, 0.0, 0.0 });
		//wall4Transform->SetScale({ 30.0, 30.0, 1.0 });
		//wall4->GetRenderComponent()->SetDebugColor({ 140.0 / 255.0, 140.0 / 255.0, 140.0 / 255.0, 1.0 });
		//wall4->GetPhysicsComponent()->IsSimulated() = false;
		//m_SceneObjects.push_back(wall4);

		EqualTri = MakeRef<EqualTriangle>();
		EqualTri->GetTransformComponent()->SetScale({10.0, 10.0, 1.0});
		m_SceneObjects.push_back(EqualTri);

		RightTri = MakeRef<RightTriangle>();
		RightTri->GetTransformComponent()->SetScale({ 10.0, 10.0, 1.0 });
		m_SceneObjects.push_back(RightTri);
		
		Square = MakeRef<Plane>();
		Square->GetTransformComponent()->SetScale({ 10.0, 10.0, 1.0 });
		m_SceneObjects.push_back(Square);

		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::Begin("Playground Scene");

		ImGui::End();
	}
}
