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
        m_ViewportCamera.reset();
        m_ActiveCamera.reset();
        ClearScene();
    }

    void Scene::BeginScene()
    {
       BaseObject::BeginScene();

        m_PxSceneDesc = new physx::PxSceneDesc(PhysicsSubsystem::m_PxPhysics->getTolerancesScale());
        m_PxSceneDesc->gravity = physx::PxVec3(0.0f, -m_Gravity, 0.0f);
        m_PxSceneDesc->cpuDispatcher	= PhysicsSubsystem::m_PxDispatcher;
        m_PxSceneDesc->filterShader = physx::PxDefaultSimulationFilterShader;

        m_PxScene = PhysicsSubsystem::m_PxPhysics->createScene(*m_PxSceneDesc);
        DE_ASSERT(m_PxScene, "Failed to create PhysX Scene");

        // Not using PhysX Controller Manager for now
        /*m_PxControllerManager = PxCreateControllerManager(*m_PxScene);
        DE_ASSERT(m_PxControllerManager, "Failed to create PhysX Controller Manager");*/
        
        physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
        PhysicsSubsystem::m_PxPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
        
        if(physx::PxPvdSceneClient* pvdClient = m_PxScene->getScenePvdClient())
        {
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }

        m_ViewportCamera->BeginScene();
        for (const auto& actor : m_Actors) actor->BeginScene();
    }

    void Scene::EndScene()
    {
        // Call EndScene on all actors
        for (const auto& actor : m_Actors) actor->EndScene();
        m_ViewportCamera.reset();
        m_ActiveCamera.reset();
        ClearScene();
        BaseObject::EndScene();
    }

    void Scene::BeginPlay()
    {
        BaseObject::BeginPlay();

        
        if (Ref<Camera> cam = FindGameCamera())
        {
            m_ActiveCamera = cam;
            DE_LOG(LogScene, Info, "Game Camera Found: {}", m_ActiveCamera->GetName())
        }
        else
        {
            m_ActiveCamera->BeginPlay(); // We need to call BeginPlay on the viewport camera manually
            DE_LOG(LogScene, Warn, "No Game Camera found. Using Viewport Camera Instead")
        }

        for (const auto& actor : m_Actors) actor->BeginPlay();
    }

    void Scene::EndPlay()
    {
        for (const auto& actor : m_Actors) actor->EndPlay();

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

        // Update Camera - This works regardless of the camer type (viewport/GameCamera)
        if (m_ActiveCamera)
        {
           m_ActiveCamera->Update(_deltaTime);
        }
    }


    bool Scene::IsPlaying() const
    { return m_IsPlaying; }

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
        for (const auto& actor : m_Actors)
        {
            if (actor->GetName() == _name)
            {
                return actor;
            }
        }

        return nullptr;
    }

    

    void Scene::SpawnActor(const Ref<Actor>& _actor)
    {
        if (!_actor)
        {
            DE_LOG(LogScene, Error, "SpawnActor: Invalid Actor")
            return;
        }
        
        _actor->BeginScene();

        if (m_IsPlaying)
            _actor->BeginPlay();

        m_Actors.push_back(std::move(_actor));
    }

    Ref<Camera> Scene::FindGameCamera() const
    {
        for (const auto& actor : m_Actors)
        {
            if (typeid(Camera) == typeid(*actor))
            {
                return std::static_pointer_cast<Camera>(actor);
            }
        }

        return nullptr;
    }

    void Scene::ClearScene()
    {
        for (const auto& actor : m_Actors)
        {
            actor->EndScene();
            if (m_IsPlaying) actor->EndPlay();
        }
        m_Actors.clear();
        m_ActorNames.clear();
    }
}
