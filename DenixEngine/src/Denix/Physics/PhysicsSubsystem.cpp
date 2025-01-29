#include "PhysicsSubsystem.h"

#include "Denix/Engine.h"
#include "Denix/Scene/Scene.h"

#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Core/Thread/JobSubsystem.h"

using namespace physx;

namespace Denix
{
    void PhysicsSubsystem::Initialize()
    {
        Subsystem::Initialize();
        DE_LOG(LogPhysics, Warn, "PhysicsSubsystem Initializing")
        m_PxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_PxAllocator, m_PxErrorCallback);
        DE_ASSERT(m_PxFoundation, "Failed to create PhysX Foundation")

        m_PxPvd = PxCreatePvd(*m_PxFoundation);
        DE_ASSERT(m_PxPvd, "Failed to create PhysX PVD")
        PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
        DE_ASSERT(transport, "Failed to create PhysX PVD Transport")
        m_PxPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

        m_PxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, PxTolerancesScale(), true, m_PxPvd);
        DE_ASSERT(m_PxPhysics, "Failed to create PhysX Physics")

        m_PxDispatcher = PxDefaultCpuDispatcherCreate(static_cast<PxU32>(JobSubsystem::GetActiveThreads()));
        DE_ASSERT(m_PxDispatcher, "Failed to create PhysX Dispatcher")

        m_PxMaterial = m_PxPhysics->createMaterial(0.5f, 0.5f, 0.5f);
        DE_ASSERT(m_PxMaterial, "Failed to create PhysX Material")

        m_PhysicsLogging = Engine::GetConfig().PhysicsLogging;
        PhysicsLoggingUpdate();

        DE_LOG(LogPhysics, Info, "PhysicsSubsystem Initialized")
    }

    void PhysicsSubsystem::Deinitialize()
    {
        DE_LOG(LogPhysics, Trace, "PhysicsSubsystem Deinitializing")

        PX_RELEASE(m_PxMaterial);
        PX_RELEASE(m_PxDispatcher);
        PX_RELEASE(m_PxPhysics);
        if (m_PxPvd)
        {
            PxPvdTransport* transport = m_PxPvd->getTransport();
            PX_RELEASE(m_PxPvd);
            PX_RELEASE(transport);
        }
        PX_RELEASE(m_PxFoundation);

        DE_LOG(LogPhysics, Trace, "PhysicsSubsystem Deinitialized")
        Subsystem::Deinitialize();
    }

    void PhysicsSubsystem::RegisterComponent(const Ref<PhysicsComponent>& _comp)
    {
        if (!_comp)
        {
            DE_LOG(LogPhysics, Error, "Invalid Componentt")
            return;
        }

        // Setup Physics Component
        auto parent = _comp->m_Parent.lock();
        const auto transform = parent->m_TransformComponent->m_Transform;
        const auto scaleHalf = transform.Scale * 0.5f;

        // Set Material
        _comp->m_PxMaterial = m_PxPhysics->createMaterial(_comp->m_StaticFriction, _comp->m_DynamicFriction,
                                                          _comp->m_Elasticity);
        DE_ASSERT(_comp->m_PxMaterial, "Failed to create PhysX Material")

        switch (_comp->m_ColliderType)
        {
        case ColliderType::Plane:
            {
                _comp->m_PxShape = m_PxPhysics->createShape(PxBoxGeometry(transform.Scale.x, 0.01f, transform.Scale.z),
                                                            *_comp->m_PxMaterial);
            }
            break;

        case ColliderType::Cube:
            {
                _comp->m_PxShape = m_PxPhysics->createShape(PxBoxGeometry(scaleHalf.x, scaleHalf.y, scaleHalf.z),
                                                            *_comp->m_PxMaterial);
            }
            break;

        case ColliderType::Sphere:
            {
                _comp->m_PxShape = m_PxPhysics->createShape(PxSphereGeometry(scaleHalf.x), *_comp->m_PxMaterial);
            }
            break;
        }

        if (!_comp->m_SimulatePhysics)
        {
            _comp->m_PxActor = m_PxPhysics->createRigidStatic(
                PxTransform(transform.Position.x, transform.Position.y, transform.Position.z));
        }
        else
        {
            if (PxRigidDynamic* pxActor = m_PxPhysics->createRigidDynamic(
                PxTransform(transform.Position.x, transform.Position.y, transform.Position.z)))
            {
                pxActor->setLinearDamping(_comp->m_LinearDrag);
                pxActor->setAngularDamping(_comp->m_AngularDrag);
                pxActor->setLinearVelocity({_comp->m_Velocity.x, _comp->m_Velocity.y, _comp->m_Velocity.z});
                pxActor->setAngularVelocity({
                    _comp->m_AngularVelocity.x, _comp->m_AngularVelocity.y, _comp->m_AngularVelocity.z
                });
                pxActor->setContactSlopCoefficient(_comp->m_PxSlopCoefficient);
                _comp->m_PxActor = pxActor;
                pxActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
                _comp->SetInertia();
            }
        }

        // Set the actor's shape & user data
        _comp->m_PxActor->userData = _comp.get();
        _comp->m_PxActor->attachShape(*_comp->m_PxShape);
        // _comp->m_PxActor->setActorFlag(, true);
        DE_ASSERT(_comp->m_PxActor, "Failed to create PhysX Actor")

        auto scene = s_Instance->m_ActiveScene.lock();
        if (scene->m_PxScene)
        {
            scene->m_PxScene->addActor(*_comp->m_PxActor);
        }

        s_Instance->m_PhysicsComponents.push_back(_comp);
    }

    void PhysicsSubsystem::UnregisterComponent(const Ref<PhysicsComponent>& _comp)
    {
        if (!_comp)
        {
            DE_LOG(LogPhysics, Error, "Invalid Commponent")
            return;
        }

        auto scene = s_Instance->m_ActiveScene.lock();
        if (scene->m_PxScene)
        {
            scene->m_PxScene->removeActor(*_comp->m_PxActor->is<PxActor>());
        }

        std::erase(s_Instance->m_PhysicsComponents, _comp);
    }

    bool PhysicsSubsystem::RayCast(const glm::vec3& _origin, const glm::vec3& _direction, float _distance,
                                   PxRaycastBuffer& _hit)
    {
        if (!s_Instance->m_ActiveScene.lock()->m_PxScene)
        {
            DE_LOG(LogPhysics, Error, "No PhysX Scene")
            return false;
        }

        return s_Instance->m_ActiveScene.lock()->m_PxScene->raycast({_origin.x, _origin.y, _origin.z},
                                                                    {_direction.x, _direction.y, _direction.z},
                                                                    _distance, _hit);
    }

    PxScene* PhysicsSubsystem::CreatePxScene(const PxSceneDesc* _sceneDesc)
    {
        return s_Instance->m_PxPhysics->createScene(*_sceneDesc);
    }

    void PhysicsSubsystem::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        Subsystem::Update(_deltaTime, _waitCounter);

        auto activeScene = s_Instance->m_ActiveScene.lock();

        if (!m_Enabled || !activeScene->IsPlaying() || SceneSubsystem::GetSceneState() == SceneState::Paused) return;

        DE_PROFILE(Physics Update)
        if (activeScene->m_PxScene)
        {
            activeScene->m_PxScene->simulate(_deltaTime);
            activeScene->m_PxScene->fetchResults(true);
        }

        DE_PROFILE_END(Physics Update)
    }

    void PhysicsSubsystem::PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        DE_PROFILE(Physics PostUpdate)
        Subsystem::PostUpdate(_deltaTime, _waitCounter);
        for (const auto& physcComp : m_PhysicsComponents) physcComp->PostUpdate(_deltaTime, _waitCounter);
        DE_PROFILE_END(Physics PostUpdate)
    }

    void PhysicsSubsystem::PhysicsLoggingUpdate()
    {
        if (m_PhysicsLogging)
        {
            m_PxFoundation->setErrorLevel(PxErrorCode::eDEBUG_INFO);
            DE_LOG(LogPhysics, Info, "Physics Logging Enabled")
        }
        else
        {
            m_PxFoundation->setErrorLevel(PxErrorCode::eNO_ERROR);
            DE_LOG(LogPhysics, Info, "Physics Logging Disabled")
        }
    }
}
