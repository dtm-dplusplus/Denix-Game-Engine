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
        
    }
    
    bool Scene::Load()
    {
        m_IsLoaded = true;

        return true;
    }

    void Scene::Unload()
    {
        m_IsLoaded = false;
    }

    void Scene::BeginScene()
    {
        m_PxSceneDesc = new physx::PxSceneDesc(PhysicsSubsystem::m_PxPhysics->getTolerancesScale());
        m_PxSceneDesc->gravity = physx::PxVec3(0.0f, -m_Gravity, 0.0f);
        m_PxSceneDesc->cpuDispatcher	= PhysicsSubsystem::m_PxDispatcher;
        m_PxSceneDesc->filterShader = physx::PxDefaultSimulationFilterShader;
        m_PxScene = PhysicsSubsystem::CreatePxScene(m_PxSceneDesc);

        if(physx::PxPvdSceneClient* pvdClient = m_PxScene->getScenePvdClient())
        {
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }
        
        m_PxScene = PhysicsSubsystem::CreatePxScene(m_PxSceneDesc);
        
        m_ActiveCamera = m_ViewportCamera;
        m_ActiveCamera->BeginScene();
        
        for (const auto& obj : m_Actors) obj->BeginScene();
    }

    void Scene::EndScene()
{
        // Call EndScene on all actors
        for (const auto& actor : m_Actors) actor->EndScene();
        
    // Release all actors from the scene
    /*if (m_PxScene)
    {
        using namespace physx;
        PxU32 numActors = m_PxScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
        std::vector<PxActor*> actors(numActors);
        m_PxScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, actors.data(), numActors);

        for (PxActor* actor : actors)
        {
            actor->userData = nullptr;
            m_PxScene->removeActor(*actor);
            actor->release();
        }
    }*/

    // Release the scene descriptor
    if (m_PxSceneDesc)
    {
        delete m_PxSceneDesc;
        m_PxSceneDesc = nullptr;
    }

    // Release the PhysX scene
    //PX_RELEASE(m_PxScene);

    BaseObject::EndScene();
}

    void Scene::BeginPlay()
    {
        BaseObject::BeginPlay();
        
        for (const auto& obj : m_Actors) obj->BeginPlay();
    }

    void Scene::EndPlay()
    {
        for (const auto& obj : m_Actors) obj->EndPlay();

        // Give camera back to viewport camera
        m_ActiveCamera = m_ViewportCamera;

        BaseObject::EndPlay();
    }

    void Scene::Update(float _deltaTime)
    {
        BaseObject::Update(_deltaTime);
    }

    void Scene::DebugUI(float _deltaTime)
    {}

    bool Scene::IsLoaded() const
    { return m_IsLoaded; }

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
        _obj->BeginScene();

        if (m_IsPlaying)
            _obj->BeginPlay();

        m_Actors.push_back(std::move(_obj));
    }

    Ref<Camera> Scene::GetGameCamera() const
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
