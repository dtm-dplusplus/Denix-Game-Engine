#include "Denix/Scene/Scene.h"

#include "Denix/Asset/Asset.h"
#include "Denix/Core/File/FileSubsystem.h"
#include "Denix/Physics/CollisionCallback.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Core/Reflection/YAMLHelper.h"

namespace Denix
{
    Scene::Scene(): BaseObject({"Scene"}), 
        m_PxScene(nullptr),
        m_CollisionCallback(nullptr),
        m_ViewportCamera{MakeRef<Camera>()},
        m_ActiveCamera{m_ViewportCamera},
        m_IsPlaying(false),
        m_DebugToolsInPlay(false),
        m_Gravity(9.81f)
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::BeginScene()
    {
        BaseObject::BeginScene();

        PxSceneDesc m_PxSceneDesc = PxSceneDesc(PhysicsSubsystem::GetPxPhysics()->getTolerancesScale());
        m_PxSceneDesc.gravity = PxVec3(0.0f, -m_Gravity, 0.0f);
        m_PxSceneDesc.cpuDispatcher = PhysicsSubsystem::GetPxDispatcher();
        m_PxSceneDesc.filterShader = PhysicsFilterShader;
        m_CollisionCallback = new CollisionCallback;
        m_PxSceneDesc.simulationEventCallback = m_CollisionCallback;


        m_PxScene = PhysicsSubsystem::PhysicsSubsystem::GetPxPhysics()->createScene(m_PxSceneDesc);
        DE_ASSERT(m_PxScene, "Failed to create PhysX Scene");

        PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
        PhysicsSubsystem::GetPxPvd()->connect(*transport, PxPvdInstrumentationFlag::eALL);

        if (PxPvdSceneClient* pvdClient = m_PxScene->getScenePvdClient())
        {
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }

        m_ViewportCamera->BeginScene();
        for (const auto& actor : m_Actors)
        {
            actor->m_SceneRef = GetRef<Scene>();
            actor->BeginScene();
        }
    }

    void Scene::EndScene()
    {
        // Call EndScene on all actors
        for (const auto& actor : m_Actors) actor->EndScene();
        m_ViewportCamera->EndScene();
        m_ViewportCamera.reset();
        m_ActiveCamera.reset();
        ClearActors();

        // Release the PhysX scene
        PX_RELEASE(m_PxScene)
        PhysicsSubsystem::GetPxPvd()->disconnect();

        BaseObject::EndScene();
    }

    void Scene::BeginPlay()
    {
        BaseObject::BeginPlay();


        if (Ref<Actor> cam = FindGameCamera())
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

        BaseObject::EndPlay();
    }

    void Scene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        BaseObject::Update(_deltaTime, _waitCounter);

        // Update Camera - This works regardless of the camer type (viewport/GameCamera)
        if (m_ActiveCamera)
        {
            m_ActiveCamera->Update(_deltaTime, _waitCounter);
        }
    }


    bool Scene::IsPlaying() const
    {
        return m_IsPlaying;
    }


    Ref<Camera> Scene::GetViewportCamera()
    {
        return m_ViewportCamera;
    }

    Ref<Actor> Scene::GetActiveCamera()
    {
        return m_ActiveCamera;
    }

    Ref<CameraComponent> Scene::GetActiveCameraComponent()
    {
        if (!m_ActiveCamera) return nullptr;

        return m_ActiveCamera->GetComponent<CameraComponent>();
    }

    std::vector<Ref<Actor>> Scene::GetSceneActors() const
    {
        return m_Actors;
    }

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

        // Validate Name. We cannont have two objects with the same name
        std::string name = _actor->GetName();
        if (m_ActorNames.contains(name))
        {
            int copy = 1;
            while (m_ActorNames.contains(name + std::to_string(copy))) copy++;
            name += std::to_string(copy);
        }
        _actor->SetName(name);
        m_ActorNames.insert(name);

        // Pass the scene reference to the actor
        _actor->m_SceneRef = GetRef<Scene>();

        // Run Begin Scene & Play. Implements any logic that needs to be run when the scene starts
        _actor->BeginScene();

        if (m_IsPlaying) _actor->BeginPlay();

        // Add the object to the scene
        m_Actors.push_back(std::move(_actor));
    }

    Ref<Actor> Scene::FindGameCamera() const
    {
        for (const auto& actor : m_Actors)
            if (actor->GetComponent<CameraComponent>()) return actor;

        return nullptr;
    }

    void Scene::ClearActors()
    {
        m_Actors.clear();
        m_ActorNames.clear();
    }
}
