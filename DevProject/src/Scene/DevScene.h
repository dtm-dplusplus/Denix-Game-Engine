#pragma once
#include "Denix/Asset/Asset.h"
#include "Denix/Scene/Scene.h"
#include "Util/ActorGridSpawner.h"
#include "PxPhysicsAPI.h"

#define PVD_HOST "127.0.0.1"

using namespace Denix;
using namespace physx;

class DevScene : public Scene
{
public:
    DevScene() = default;
    ~DevScene() override = default;

    PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity);
    void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
    void initPhysics(bool interactive);
    void stepPhysics(bool interactive);
    void cleanupPhysics(bool interactive);
    void BeginScene() override;
    void EndScene() override;
    void Update(float _deltaTime) override;
    void DebugUI(float _deltaTime) override;

    PxDefaultAllocator		gAllocator;
    PxDefaultErrorCallback	gErrorCallback;
    PxFoundation*			gFoundation = NULL;
    PxPhysics*				gPhysics	= NULL;
    PxDefaultCpuDispatcher*	gDispatcher = NULL;
    PxScene*					gScene		= NULL;
    PxMaterial*				gMaterial	= NULL;
    PxPvd*					gPvd        = NULL;

    PxReal stackZ = 10.0f;

    Ref<Actor> m_DyCube;
    Ref<Actor> m_StatCube;
};