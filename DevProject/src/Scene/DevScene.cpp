
#include "DevScene.h"

#include "imgui.h"
#include "Denix/Core/TimerSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"

using namespace Denix;

PxRigidDynamic* DevScene::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity=PxVec3(0))
{
	
	PxRigidDynamic* dynamic = PxCreateDynamic(*PhysicsSubsystem::gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	m_PxScene->addActor(*dynamic);
	return dynamic;
}

void DevScene::BeginScene()
{
	Scene::BeginScene();

	ClearScene();

	// Disabled for now - Investigage manual thread control
	//PhysicsSubsystem::gDispatcher = PxDefaultCpuDispatcherCreate(1);
	//sceneDesc.cpuDispatcher	= PhysicsSubsystem::gDispatcher;
	//sceneDesc.filterShader	= PxDefaultSimulationFilterShader;
	//m_PxScene = PhysicsSubsystem::gPhysics->createScene(sceneDesc);
	
	// Dynamic Cube
	{
		m_DyActor = SpawnActor<Sphere>();
		m_DyActor->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		m_DyActor->GetTransformComponent()->SetPosition({0.0f, 10.0f, 0.0f});
	}

	// Static Cube
	{
		m_StatActor = SpawnActor<Plane>();
		m_StatActor->GetTransformComponent()->SetMoveability(Moveability::Static);
		m_StatActor->GetTransformComponent()->GetScale() = {4.0f, 0.1f, 4.0f};
	}
}

void DevScene::EndScene()
{
	Scene::EndScene();
}

void DevScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);

	if (!IsPlaying()) return;

	if (m_DyActor)
	{
		PxActor* dyActor;
		m_PxScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, &dyActor, 1);
		if (const PxRigidDynamic* actor = dyActor->is<PxRigidDynamic>())
		{
			PxTransform transform = actor->getGlobalPose();
			glm::vec3 position = {transform.p.x, transform.p.y, transform.p.z};
			glm::vec3 rotation = {transform.q.x, transform.q.y, transform.q.z};

			PxShape* shape;
			actor->getShapes(&shape, 1);
			glm::vec3 scale = m_DyActor->GetTransformComponent()->GetScale();

			if (shape)
			{
				switch (m_DyActor->GetPhysicsComponent()->m_ColliderType)
				{
				case ColliderType::Plane:
					{
					
					} 
				
				case ColliderType::Cube:
					{
						PxGeometryHolder geometry = shape->getGeometry();
						PxBoxGeometry box = geometry.box();
						scale = {box.halfExtents.x, box.halfExtents.y, box.halfExtents.z} * 2.0f;
					} break;
				case ColliderType::Sphere:
					{
						PxGeometryHolder geometry = shape->getGeometry();
						PxSphereGeometry sphere = geometry.sphere();
						scale = glm::vec3(sphere.radius) * 2.0f;
					
					} break;
				}
			}
			m_DyActor->GetTransformComponent()->SetPosition(position);
			m_DyActor->GetTransformComponent()->SetRotation(rotation);
			m_DyActor->GetTransformComponent()->SetScale(scale);
		}
	}
	

	if (m_StatActor)
	{
		PxActor* statActor;
	m_PxScene->getActors(PxActorTypeFlag::eRIGID_STATIC, &statActor, 1);
	if (statActor)
	{
		if (const PxRigidStatic* actor = statActor->is<PxRigidStatic>())
		{
			PxTransform transform = actor->getGlobalPose();
			glm::vec3 position = {transform.p.x, transform.p.y, transform.p.z};
			glm::vec3 rotation = {/*transform.q.x*/ 20.f, transform.q.y, transform.q.z};
		
			PxShape* shape;
			actor->getShapes(&shape, 1);
			glm::vec3 scale = m_StatActor->GetTransformComponent()->GetScale();

			if (shape)
			{
				PxGeometryHolder geometry = shape->getGeometry();
				PxBoxGeometry box = geometry.box();
				scale = {box.halfExtents.x, box.halfExtents.y , box.halfExtents.z } * 2.0f;
			}
		
			m_StatActor->GetTransformComponent()->SetPosition(position);
			m_StatActor->GetTransformComponent()->SetRotation(rotation);
			m_StatActor->GetTransformComponent()->SetScale(scale);
		}
	}
	}
}

void DevScene::DebugUI(float _deltaTime)
{
	Scene::DebugUI(_deltaTime);

	ImGui::Begin(GetName().c_str());
	ImGui::End();
}