#include "PhysicsSubsystem.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Physics/Collider.h"
#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"

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
        m_PxPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

        m_PxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, PxTolerancesScale(), true, m_PxPvd);
        DE_ASSERT(m_PxPhysics, "Failed to create PhysX Physics")
        
        m_PxDispatcher = PxDefaultCpuDispatcherCreate((PxU32)JobSubsystem::GetActiveThreads());
        DE_ASSERT(m_PxDispatcher, "Failed to create PhysX Dispatcher")
        
        m_PxMaterial = m_PxPhysics->createMaterial(0.5f, 0.5f, 0.5f);
        DE_ASSERT(m_PxMaterial, "Failed to create PhysX Material")
        
        // Set the logging level to suppress debug messages for now
        //m_PxFoundation->setErrorLevel(PxErrorCode::eDEBUG_INFO);
        
        DE_LOG(LogPhysics, Info, "PhysicsSubsystem Initialized")
    }

    void PhysicsSubsystem::Deinitialize()
    {
        DE_LOG(LogPhysics, Trace, "PhysicsSubsystem Deinitializing")
        
        PX_RELEASE(m_PxMaterial);
        PX_RELEASE(m_PxDispatcher);
        PX_RELEASE(m_PxPhysics); 
        if(m_PxPvd)
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

        if (!_comp->m_PxActor)
        {
            DE_LOG(LogPhysics, Error, "Invalid PxActor")
            return;
        }
        
        auto scene = s_Instance->m_ActiveScene.lock();
         if (scene->m_PxScene)
         {
             scene->m_PxScene->addActor(*_comp->m_PxActor);
         }
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
          scene->m_PxScene->removeActor(*_comp->m_PxActor->is<physx::PxActor>());
        }
    }

    bool PhysicsSubsystem::RayCast(const glm::vec3& _origin, const glm::vec3& _direction, float _distance,
        physx::PxRaycastBuffer& _hit)
    {
        if (!s_Instance->m_ActiveScene.lock()->m_PxScene)
        {
            DE_LOG(LogPhysics, Error, "No PhysX Scene")
            return false;
        }

        return s_Instance->m_ActiveScene.lock()->m_PxScene->raycast({ _origin.x, _origin.y, _origin.z },
            { _direction.x, _direction.y, _direction.z }, _distance, _hit);
    }

    void PhysicsSubsystem::Update(float _deltaTime)
    {
        DE_PROFILE(Physics Update)
        auto activeScene = s_Instance->m_ActiveScene.lock();
        
        if (!m_Enabled || !activeScene->IsPlaying() ||
            SceneSubsystem::GetSceneState() == SceneState::Paused)
        {
            DE_PROFILE_END(Physics Update)
            return;
        }
        
        if (activeScene->m_PxScene)
        {
            activeScene->m_PxScene->simulate(_deltaTime);
            activeScene->m_PxScene->fetchResults(true);
        }

        DE_PROFILE_END(Physics Update)
    }

    physx::PxScene* PhysicsSubsystem::CreatePxScene(const physx::PxSceneDesc* _sceneDesc)
    {
        return s_Instance->m_PxPhysics->createScene(*_sceneDesc);
    }
}