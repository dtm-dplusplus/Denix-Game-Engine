
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

void DevScene::createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	/*PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	Ref<Cube> cube = SpawnActor<Cube>();
	for(PxU32 i=0; i<size;i++)
	{
		for(PxU32 j=0;j<size-i;j++)
		{
			PxTransform localTm(PxVec3(PxReal(j*2) - PxReal(size-i), PxReal(i*2+1), 0) * halfExtent);
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			gScene->addActor(*body);
		}
	}
	shape->release();*/

	// Floor
	/*{
		PxShape* shape = gPhysics->createShape(PxPlaneGeometry(), *gMaterial);
		PxRigidStatic* body = PxCreatePlane(*gPhysics, PxPlane(0,1,0,1), *gMaterial);
		body->attachShape(*shape);
		// PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		gScene->addActor(*body);
		m_Plane = SpawnActor<Plane>();
		shape->release();
	}*/

	// Dynamic Cube
	{
		m_DyCube = SpawnActor<Cube>();
		PxShape* shape = PhysicsSubsystem::gPhysics->createShape(PxBoxGeometry(.5, .5, .5), *gMaterial);
		PxTransform localTm({0.0f, 10.0f, 0.0f});
		PxRigidDynamic* body = PhysicsSubsystem::gPhysics->createRigidDynamic(t.transform(localTm));
		body->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		gScene->addActor(*body);
		shape->release();
	}

	// Static Cube
	{
		m_StatCube = SpawnActor<Cube>();
		PxShape* shape = PhysicsSubsystem::gPhysics->createShape(PxBoxGeometry(.5, .5, .5), *gMaterial);
		PxTransform localTm({0.0f, 0.0f, 0.0f});
		PxRigidStatic* body = PhysicsSubsystem::gPhysics->createRigidStatic(t.transform(localTm));
		body->attachShape(*shape);
		gScene->addActor(*body);
		shape->release();
	}
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

	
	
	//for(PxU32 i=0;i<5;i++)
	createStack(PxTransform(PxVec3(0,0,10.0f)), 10, 2.0f);

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
	stepPhysics(false);

	PxActor* cube;
	gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, &cube, 1);
	if (const PxRigidDynamic* actor = cube->is<PxRigidDynamic>())
	{
		PxTransform transform = actor->getGlobalPose();
		glm::vec3 position = {transform.p.x, transform.p.y, transform.p.z};
		glm::vec3 rotation = {transform.q.x, transform.q.y, transform.q.z};
		glm::vec3 scale = {1.0f, 1.0f, 1.0f};
		m_DyCube->GetTransformComponent()->SetPosition(position);
		m_DyCube->GetTransformComponent()->SetRotation(rotation);
		m_DyCube->GetTransformComponent()->SetScale(scale);
	}
	
	/*PxActor* plane;
	gScene->getActors(PxActorTypeFlag::eRIGID_STATIC, &plane, 1);
	if (const PxRigidStatic* actor = plane->is<PxRigidStatic>())
	{
		PxTransform transform = actor->getGlobalPose();
		glm::vec3 position = {transform.p.x, transform.p.y, transform.p.z};
		glm::vec3 rotation = {transform.q.x, transform.q.y, transform.q.z};
		glm::vec3 scale = {1.0f, 1.0f, 1.0f};
		m_Plane->GetTransformComponent()->SetPosition(position);
		m_Plane->GetTransformComponent()->SetRotation(rotation);
		m_Plane->GetTransformComponent()->SetScale(scale);
	}*/

	PxActor* plane;
	PxU32 planeActor = gScene->getActors(PxActorTypeFlag::eRIGID_STATIC, &plane, 1);
	if (const PxRigidStatic* actor = plane->is<PxRigidStatic>())
	{
		PxTransform transform = actor->getGlobalPose();
		glm::vec3 position = {transform.p.x, transform.p.y, transform.p.z};
		glm::vec3 rotation = {transform.q.x, transform.q.y, transform.q.z};
		glm::vec3 scale = {1.0f, 1.0f, 1.0f};
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