#include "Denix/Scene/Scene.h"

#include "Denix/Asset/Asset.h"

namespace Denix
{
    Scene::Scene(): BaseObject({"Scene"}),
                    m_ViewportCamera{ MakeRef<Camera>() },
                    m_ActiveCamera{ m_ViewportCamera }
    {
    }

    Scene::Scene(const ObjectInit& _objInit): BaseObject(_objInit),
                                              m_ViewportCamera{ MakeRef<Camera>() },
                                              m_ActiveCamera{ m_ViewportCamera }
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
        m_ActiveCamera = m_ViewportCamera;

        for (const auto& obj : m_Actors) obj->BeginScene();
    }

    void Scene::EndScene()
    {
        for (const auto& obj : m_Actors) obj->EndScene();
    }

    void Scene::BeginPlay()
    {
        for (const auto& obj : m_Actors) obj->BeginPlay();
    }

    void Scene::EndPlay()
    {
        for (const auto& obj : m_Actors) obj->EndPlay();

        // Give camera back to viewport camera
        m_ActiveCamera = m_ViewportCamera;
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

    std::vector<Ref<Actor>> Scene::GetSceneObjects() const
    { return m_Actors; }

    std::vector<Ref<Actor>>& Scene::GetSceneObjects()
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
