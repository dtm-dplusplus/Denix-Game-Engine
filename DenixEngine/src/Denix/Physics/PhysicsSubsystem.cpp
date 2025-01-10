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

        m_PxPvd = PxCreatePvd(*m_PxFoundation);
        PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
        m_PxPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

        m_PxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, PxTolerancesScale(), true, m_PxPvd);

        m_PxDispatcher = PxDefaultCpuDispatcherCreate((PxU32)JobSubsystem::GetActiveThreads());
        
        m_PxMaterial = m_PxPhysics->createMaterial(0.5f, 0.5f, 0.5f);
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

    void PhysicsSubsystem::UnregisterPxActor(const physx::PxRigidActor* _actor)
    {
        if (!_actor)
        {
            DE_LOG(LogPhysics, Error, "Invalid PxActor")
            return;
        }

        auto scene = s_Instance->m_ActiveScene.lock();
        if (scene->m_PxScene)
        {
          //  scene->m_PxScene->removeActor(*_actor);
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
        
        DE_PROFILE(Physics Simulation)
        if (activeScene->m_PxScene)
        {
            activeScene->m_PxScene->simulate(_deltaTime);
            activeScene->m_PxScene->fetchResults(true);
        }
        DE_PROFILE_END(Physics Simulation)

        DE_PROFILE_END(Physics Update)
    }

    physx::PxScene* PhysicsSubsystem::CreatePxScene(const physx::PxSceneDesc* _sceneDesc)
    {
        return s_Instance->m_PxPhysics->createScene(*_sceneDesc);
    }
}