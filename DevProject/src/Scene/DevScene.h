#pragma once
#include "Denix/Asset/Asset.h"
#include "Denix/Scene/Scene.h"
#include "Util/ActorGridSpawner.h"

#include "PxPhysicsAPI.h"

using namespace Denix;
using namespace physx;

class DevScene : public Scene
{
public:
    DevScene() = default;
    ~DevScene() override = default;

    PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity);
    void BeginScene() override;
    void EndScene() override;
    void Update(float _deltaTime) override;
    void DebugUI(float _deltaTime) override;


    PxScene*	gScene		= NULL;
    PxMaterial*	gMaterial	= NULL;

    Ref<Actor> m_DyActor;
    Ref<Actor> m_StatActor;
};