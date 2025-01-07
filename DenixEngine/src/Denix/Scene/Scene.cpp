#include "Denix/Scene/Scene.h"

#include "Denix/Asset/Asset.h"
#include "Denix/Physics/PhysicsSubsystem.h"

namespace Denix
{
   

    Scene::Scene(): BaseObject({"Scene"}), m_PxScene(nullptr), m_PxSceneDesc(nullptr),
                    m_ViewportCamera{MakeRef<Camera>()},
                    m_ActiveCamera{m_ViewportCamera}
    {
        
    }

    Scene::~Scene()
    {
        m_GameCamera.reset();
        m_ViewportCamera.reset();
        m_ActiveCamera.reset();
        ClearScene();
    }

    void Scene::BeginScene()
    {
       BaseObject::BeginScene();

        
        for (const auto& obj : m_Actors) obj->BeginScene();
    }

    void Scene::EndScene()
    {
        // Call EndScene on all actors
        for (const auto& actor : m_Actors) actor->EndScene();

        m_GameCamera.reset();
        m_ViewportCamera.reset();
        m_ActiveCamera.reset();
        ClearScene();
        BaseObject::EndScene();
    }

    void Scene::BeginPlay()
    {
        BaseObject::BeginPlay();

        m_PxSceneDesc = new physx::PxSceneDesc(PhysicsSubsystem::m_PxPhysics->getTolerancesScale());
        m_PxSceneDesc->gravity = physx::PxVec3(0.0f, -m_Gravity, 0.0f);
        m_PxSceneDesc->cpuDispatcher	= PhysicsSubsystem::m_PxDispatcher;
        m_PxSceneDesc->filterShader = physx::PxDefaultSimulationFilterShader;
        m_PxScene = PhysicsSubsystem::m_PxPhysics->createScene(*m_PxSceneDesc);

        physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
        PhysicsSubsystem::m_PxPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
        
        if(physx::PxPvdSceneClient* pvdClient = m_PxScene->getScenePvdClient())
        {
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }
        
        if (m_GameCamera)
        {
            m_ActiveCamera = m_GameCamera;
            DE_LOG(LogScene, Info, "Game Camera Found: {}", m_ActiveCamera->GetName())
        }
        else
        {
            DE_LOG(LogScene, Warn, "No Game Camera found. Using Viewport Camera Instead")
        }
        
        for (const auto& obj : m_Actors) obj->BeginPlay();
    }

    void Scene::EndPlay()
    {
        for (const auto& obj : m_Actors) obj->EndPlay();

        // Iterate over all actors in the scene
        physx::PxActorTypeFlags actorFlags = physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC;
        physx::PxU32 numActors = m_PxScene->getNbActors(actorFlags);

        if (numActors > 0)
            {
            std::vector<physx::PxActor*> actors(numActors);
            m_PxScene->getActors(actorFlags, actors.data(), numActors);
            // Release each actor
            for (physx::PxActor* actor : actors) {
                if (actor) {
                    actor->userData = nullptr;
                    m_PxScene->removeActor(*actor);
                    actor->release();
                }
            }
        }
        
        m_Actors.clear();
        m_ActorNames.clear();
        
        // Release the PhysX scene
        PX_RELEASE(m_PxScene);

        if (m_PxSceneDesc)
        {
            delete m_PxSceneDesc;
            m_PxSceneDesc = nullptr;
        }

        PhysicsSubsystem::m_PxPvd->disconnect();
        
        BaseObject::EndPlay();
    }

    void Scene::Update(float _deltaTime)
    {
        BaseObject::Update(_deltaTime);
    }


    bool Scene::IsOpen() const
    { return m_IsOpen; }

    bool Scene::IsPlaying() const
    { return m_IsPlaying; }

    float Scene::GetGravity() const
    { return m_Gravity; }

    float& Scene::GetGravity()
    { return m_Gravity; }

    Ref<Camera> Scene::GetViewportCamera()
    { return m_ViewportCamera; }

    Ref<Camera> Scene::GetActiveCamera()
    { return m_ActiveCamera; }

    std::vector<Ref<Actor>> Scene::GetSceneActors() const
    { return m_Actors; }

    std::vector<Ref<Actor>>& Scene::GetSceneActors()
    { return m_Actors; }

    Ref<Actor> Scene::GetActorByName(const std::string& _name) const
    {
        for (const auto& obj : m_Actors)
        {
            if (obj->GetName() == _name)
            {
                return obj;
            }
        }

        return nullptr;
    }

    

    void Scene::SpawnActor(const Ref<Actor>& _obj)
    {
        if (!_obj)
        {
            DE_LOG(LogScene, Error, "SpawnActor: Invalid Actor")
            return;
        }
        
        _obj->BeginScene();

        if (m_IsPlaying)
            _obj->BeginPlay();

        m_Actors.push_back(std::move(_obj));
    }

    Ref<Camera> Scene::FindGameCamera() const
    {
        for (const auto& obj : m_Actors)
        {
            if (typeid(Camera) == typeid(*obj))
            {
                return std::static_pointer_cast<Camera>(obj);
            }
        }

        return nullptr;
    }

    void Scene::ClearScene()
    {
        m_Actors.clear();
        m_ActorNames.clear();
    }
}
