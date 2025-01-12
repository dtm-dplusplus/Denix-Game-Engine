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
        m_PxFoundation->setErrorLevel(PxErrorCode::eDEBUG_INFO);
        
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
    
    void PhysicsSubsystem::RegisterPxActor(physx::PxRigidActor* _actor)
    {
        if (!_actor)
        {
            DE_LOG(LogPhysics, Error, "Invalid PxActor")
            return;
        }

        auto scene = s_Instance->m_ActiveScene.lock();
         if (scene->m_PxScene)
         {
             scene->m_PxScene->addActor(*_actor);
         }
    }

    void PhysicsSubsystem::UnregisterPxActor(physx::PxRigidActor* _actor)
    {
        if (!_actor)
        {
            DE_LOG(LogPhysics, Error, "Invalid PxActor")
            return;
        }

        auto scene = s_Instance->m_ActiveScene.lock();
        if (scene->m_PxScene)
        {
          scene->m_PxScene->removeActor(*_actor->is<physx::PxActor>());
        }
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