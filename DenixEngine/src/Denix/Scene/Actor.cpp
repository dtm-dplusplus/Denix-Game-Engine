#include "Actor.h"

#include "Denix/Scene/Scene.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Core/Reflection/YAMLHelper.h"
#include "Denix/Core/Reflection/ReflectionSubsystem.h"

namespace Denix
{
    Actor::Actor()
    {
        m_TransformComponent = AddComponent<TransformComponent>();
        m_ModelComponent = AddComponent<ModelComponent>();
        m_RenderComponent = AddComponent<RenderComponent>();
        m_PhysicsComponent = AddComponent<PhysicsComponent>();
    }

    Actor::Actor(const ObjectInit& _object_init) : BaseObject(_object_init)
    {
        m_TransformComponent = AddComponent<TransformComponent>();
        m_ModelComponent = AddComponent<ModelComponent>();
        m_RenderComponent = AddComponent<RenderComponent>();
        m_PhysicsComponent = AddComponent<PhysicsComponent>();
    }

    void Actor::Serialize(YAML::Emitter& _out)
    {
        BaseObject::Serialize(_out);

        _out << YAML::Key << "m_Components" << YAML::BeginMap;
        int i = 0;
        for (const auto& comp: m_Components)
        {
            _out << YAML::Key << comp->GetClassNameDE() << YAML::BeginMap;
            comp->Serialize(_out);
            _out << YAML::EndMap;
        }
        _out << YAML::EndMap;
    }

    void Actor::Deserialize(const YAML::Node& _in)
    {
        // Object instantiation is done in the SceneSubsystem so we don't need to do it here
        BaseObject::Deserialize(_in);

        m_Components.clear();
        m_ComponentMap.clear();
        m_ModelComponent.reset();
        m_TransformComponent.reset();
        m_RenderComponent.reset();
        m_PhysicsComponent.reset();
        
        // Desirialize the components
        if (const YAML::Node& compMap = _in["m_Components"]; compMap.IsDefined())
        {
            for (const auto& compNode: compMap)
            {
                // Validate Node
                if (!compNode.first.IsDefined()) continue;
                if (!compNode.second.IsDefined()) continue;
                
               // Create the component
               if (Ref<Component> comp = ReflectionSubsystem::Create<Component>(compNode.first.as<std::string>()))
               {
                   comp->Deserialize(compNode.second);

                   // Cache for common components
                   if (comp->GetClassNameDE() == "TransformComponent") m_TransformComponent = CastRef<TransformComponent>(comp);
                   else if (comp->GetClassNameDE() == "ModelComponent") m_ModelComponent = CastRef<ModelComponent>(comp);
                    else if (comp->GetClassNameDE() == "RenderComponent") m_RenderComponent = CastRef<RenderComponent>(comp);
                   else if (comp->GetClassNameDE() == "PhysicsComponent") m_PhysicsComponent = CastRef<PhysicsComponent>(comp);

                   AddComponent(comp);
               }
            }
        }
    }

    void Actor::AddComponent(const Ref<Component>& _comp)
    {
        if (!_comp) return;

        // Peform BeginPlay Checks
        // If we have a scene refrence its safe to assume we are not calling this from constructor
        if (Ref<Scene> scene = m_SceneRef.lock())
        {
            _comp->BeginScene();
            if (scene->IsPlaying()) _comp->BeginPlay();
        }
        m_Components.push_back(_comp);
        m_ComponentMap[_comp->GetClassNameDE()] = _comp;
    }

    void Actor::Destroy()
    {
        BaseObject::Destroy();
    }

    void Actor::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        BaseObject::Update(_deltaTime, _waitCounter);

        for(const auto& component : m_Components) component->Update(_deltaTime, _waitCounter);

        // Physics Callback Events
        if (m_PhysicsComponent->CollisionDetectionEnabled())
        {
            for (const auto& col : m_PhysicsComponent->m_CollisionData)
            {
                if (col.m_Actors[0] && col.m_Actors[1])
                {
                    int otherIndex = 0;
                    
                    if (col.m_Actors[0] == m_PhysicsComponent.get()) otherIndex = 1;

                    if (col.CollisionEnter) OnCollisionEnter(col.m_Actors[otherIndex]->GetParent(), col.Normal, col.Point);
                    else OnCollisionExit(col.m_Actors[otherIndex]->GetParent(), col.Normal, col.Point);
                }
            }
        }

        m_PhysicsComponent->m_CollisionData.clear();
    }

    void Actor::BeginScene()
    {
        BaseObject::BeginScene();

        for (const auto& component : m_Components)
        {
            component->m_Parent = shared_from_this();
            component->BeginScene();
        }
    }

    void Actor::EndScene()
    {
        for (const auto& component : m_Components) component->EndScene();

        BaseObject::EndScene();
    }

    void Actor::BeginPlay()
    {
        BaseObject::BeginPlay();

        for (const auto& component : m_Components) component->BeginPlay();
    }

    void Actor::EndPlay()
    {
        for (const auto& component : m_Components) component->EndPlay();

        BaseObject::EndPlay();
    }
}
