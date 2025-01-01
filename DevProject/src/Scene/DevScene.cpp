
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
	gScene->addActor(*dynamic);
	return dynamic;
}

void DevScene::stepPhysics(bool interactive)
{
	gScene->simulate(TimerSubsystem::GetDeltaTime());
	gScene->fetchResults(true);
}

void DevScene::BeginScene()
{
	Scene::BeginScene();

	ClearScene();
	
	PxSceneDesc sceneDesc(PhysicsSubsystem::gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	PhysicsSubsystem::gDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.cpuDispatcher	= PhysicsSubsystem::gDispatcher;
	sceneDesc.filterShader	= PxDefaultSimulationFilterShader;
	gScene = PhysicsSubsystem::gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if(pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = PhysicsSubsystem::gPhysics->createMaterial(0.5f, 0.5f, 0.2f);

	
	
	// Dynamic Cube
	{
		m_DyCube = SpawnActor<Cube>();
		PxVec3 scale = {m_DyCube->GetTransformComponent()->GetScale().x, m_DyCube->GetTransformComponent()->GetScale().y, m_DyCube->GetTransformComponent()->GetScale().z};
		m_DyCube->GetPhysicsComponent()->m_PxShape = PhysicsSubsystem::gPhysics->createShape(PxBoxGeometry(scale), *gMaterial);
		m_DyCube->GetPhysicsComponent()->m_PxTransform = PxTransform(PxVec3(0, 10, 0));
		PxRigidDynamic* body = PhysicsSubsystem::gPhysics->createRigidDynamic(m_DyCube->GetPhysicsComponent()->m_PxTransform);
		body->attachShape(*m_DyCube->GetPhysicsComponent()->m_PxShape);
		PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		gScene->addActor(*body);
		m_DyCube->GetPhysicsComponent()->m_PxShape->release();
	}

	// Static Cube
	{
		m_StatCube = SpawnActor<Cube>();
		m_StatCube->GetTransformComponent()->GetScale() = {2.0f, 0.5f, 2.0f};
		m_StatCube->GetPhysicsComponent()->m_PxShape = PhysicsSubsystem::gPhysics->createShape(PxBoxGeometry(2, .5, 2), *gMaterial);
		m_StatCube->GetPhysicsComponent()->m_PxActor = PhysicsSubsystem::gPhysics->createRigidStatic(m_StatCube->GetPhysicsComponent()->m_PxTransform);
		m_StatCube->GetPhysicsComponent()->m_PxActor->attachShape(*m_StatCube->GetPhysicsComponent()->m_PxShape);
		gScene->addActor(*m_StatCube->GetPhysicsComponent()->m_PxActor);
		m_StatCube->GetPhysicsComponent()->m_PxShape->release();
	}

	/*if(!interactive)
		createDynamic(PxTransform(PxVec3(0,40,100)), PxSphereGeometry(10), PxVec3(0,-50,-100));*/
}

void DevScene::EndScene()
{
	Scene::EndScene();

	PX_RELEASE(gScene);
}

void DevScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);

	if (!IsPlaying()) return;
	
	stepPhysics(false);

	PxActor* cube;
	gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, &cube, 1);
	if (const PxRigidDynamic* actor = cube->is<PxRigidDynamic>())
	{
		PxTransform transform = actor->getGlobalPose();
		glm::vec3 position = {transform.p.x, transform.p.y, transform.p.z};
		glm::vec3 rotation = {transform.q.x, transform.q.y, transform.q.z};

		PxShape* shape;
		actor->getShapes(&shape, 1);
		glm::vec3 scale = m_DyCube->GetTransformComponent()->GetScale();

		if (shape)
		{
			PxGeometryHolder geometry = shape->getGeometry();
			PxBoxGeometry box = geometry.box();
			scale = {box.halfExtents.x * 2, box.halfExtents.y * 2, box.halfExtents.z * 2};
			m_DyCube->GetTransformComponent()->SetPosition(position);
			m_DyCube->GetTransformComponent()->SetRotation(rotation);
			m_DyCube->GetTransformComponent()->SetScale(scale);
		}
		m_DyCube->GetTransformComponent()->SetPosition(position);
		m_DyCube->GetTransformComponent()->SetRotation(rotation);
		m_DyCube->GetTransformComponent()->SetScale(scale);
	}
	
	PxActor* plane;
	gScene->getActors(PxActorTypeFlag::eRIGID_STATIC, &plane, 1);
	if (const PxRigidStatic* actor = plane->is<PxRigidStatic>())
	{
		PxTransform transform = actor->getGlobalPose();
		glm::vec3 position = {transform.p.x, transform.p.y, transform.p.z};
		glm::vec3 rotation = {transform.q.x, transform.q.y, transform.q.z};
		
		PxShape* shape;
		actor->getShapes(&shape, 1);
		glm::vec3 scale = m_StatCube->GetTransformComponent()->GetScale();

		if (shape)
		{
			PxGeometryHolder geometry = shape->getGeometry();
			PxBoxGeometry box = geometry.box();
			scale = {box.halfExtents.x * 2, box.halfExtents.y * 2, box.halfExtents.z * 2};
			m_StatCube->GetTransformComponent()->SetPosition(position);
			m_StatCube->GetTransformComponent()->SetRotation(rotation);
			m_StatCube->GetTransformComponent()->SetScale(scale);
		}
		
		m_StatCube->GetTransformComponent()->SetPosition(position);
		m_StatCube->GetTransformComponent()->SetRotation(rotation);
		m_StatCube->GetTransformComponent()->SetScale(scale);
	}
}

void DevScene::DebugUI(float _deltaTime)
{
	Scene::DebugUI(_deltaTime);

	ImGui::Begin(GetName().c_str());
	ImGui::End();
}