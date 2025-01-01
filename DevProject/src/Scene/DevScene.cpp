
#include "DevScene.h"

#include <random>

#include "imgui.h"

using namespace Denix;

PxRigidDynamic* DevScene::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity=PxVec3(0))
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
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
		PxShape* shape = gPhysics->createShape(PxBoxGeometry(.5, .5, .5), *gMaterial);
		PxTransform localTm({0.0f, 10.0f, 0.0f});
		PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
		body->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		gScene->addActor(*body);
		shape->release();
	}

	// Static Cube
	{
		m_StatCube = SpawnActor<Cube>();
		PxShape* shape = gPhysics->createShape(PxBoxGeometry(.5, .5, .5), *gMaterial);
		PxTransform localTm({0.0f, 0.0f, 0.0f});
		PxRigidStatic* body = gPhysics->createRigidStatic(t.transform(localTm));
		body->attachShape(*shape);
		gScene->addActor(*body);
		shape->release();
	}
}

void DevScene::initPhysics(bool interactive)
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.cpuDispatcher	= gDispatcher;
	sceneDesc.filterShader	= PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if(pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.2f);

	
	
	//for(PxU32 i=0;i<5;i++)
	createStack(PxTransform(PxVec3(0,0,stackZ-=10.0f)), 10, 2.0f);

	if(!interactive)
		createDynamic(PxTransform(PxVec3(0,40,100)), PxSphereGeometry(10), PxVec3(0,-50,-100));
}

void DevScene::stepPhysics(bool interactive)
{
	gScene->simulate(1.0f/60.0f);
	gScene->fetchResults(true);
}

void DevScene::cleanupPhysics(bool interactive)
{
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	if(gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		PX_RELEASE(gPvd);
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);

	printf("SnippetHelloWorld done.\n");
}

void DevScene::BeginScene()
{
	Scene::BeginScene();

	ClearScene();
	
	initPhysics(false);
}

void DevScene::EndScene()
{
	Scene::EndScene();

	cleanupPhysics(false);
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