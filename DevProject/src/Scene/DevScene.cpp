
#include "DevScene.h"

#include "imgui.h"
#include "Denix/Core/TimerSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"

using namespace Denix;

PxRigidDynamic* DevScene::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity=PxVec3(0))
{
	
	PxRigidDynamic* dynamic = PxCreateDynamic(*PhysicsSubsystem::m_PxPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	m_PxScene->addActor(*dynamic);
	return dynamic;
}

void DevScene::BeginScene()
{
	Scene::BeginScene();

	ClearScene();

	// Dynamic Cube
	/*{
		m_DyActor = SpawnActor<Cube>();
		m_DyActor->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		m_DyActor->GetTransformComponent()->SetPosition({0.0f, 5.0f, 0.0f});
		m_DyActor->GetPhysicsComponent()->GetAngularVelocity().z =- 5.0f;
	}*/

	for (int i = 0; i < 1000; i++)
	{
		Ref<Actor> actor = SpawnActor<Sphere>();
		actor->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		actor->GetTransformComponent()->SetPosition({0.0f, 11.0f + i * 2.0f, 0.0f});
		actor->GetPhysicsComponent()->GetAngularVelocity() = {Math::RandF(-5,5),Math::RandF(-5,5),Math::RandF(-5,5)};
	}
	// Static Cube
	{
		m_StatActor = SpawnActor<Cube>();
		m_StatActor->GetTransformComponent()->SetMoveability(Moveability::Static);
		m_StatActor->GetTransformComponent()->GetScale() = {10.0f, 1.0f, 10.0f};
	}

	// Static Plane
	/*{
		m_StatActor = SpawnActor<Plane>();
		m_StatActor->GetTransformComponent()->SetMoveability(Moveability::Static);
		m_StatActor->GetTransformComponent()->GetScale() = {4.0f, 0.01f, 4.0f};
	}*/
}

void DevScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);

}

void DevScene::DebugUI(float _deltaTime)
{
	Scene::DebugUI(_deltaTime);

	ImGui::Begin(GetName().c_str());
	ImGui::End();
}