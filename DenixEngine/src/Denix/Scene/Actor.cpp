#include "Actor.h"

#include "Denix/Asset/AssetSubsystem.h"

#include "yaml-cpp/yaml.h"
#include "Denix/Core/Reflection/YAMLHelper.h"
#include "Denix/Asset/Asset.h"

namespace Denix
{
    Actor::Actor()
    {
        m_TransformComponent = AddComponent<TransformComponent>();
        m_MeshComponent = AddComponent<MeshComponent>();
        m_RenderComponent = AddComponent<RenderComponent>();
        m_PhysicsComponent = AddComponent<PhysicsComponent>();
    }

    Actor::Actor(const ObjectInit& _object_init) : BaseObject(_object_init)
    {
        m_TransformComponent = AddComponent<TransformComponent>();
        m_MeshComponent = AddComponent<MeshComponent>();
        m_RenderComponent = AddComponent<RenderComponent>();
        m_PhysicsComponent = AddComponent<PhysicsComponent>();
    }

    void Actor::Serialize(YAML::Emitter& _out)
    {
        BaseObject::Serialize(_out);
        
        // Render Component
        _out << YAML::Newline << YAML::Comment("Render Component");
        _out << YAML::Key << "m_RenderComponent" << YAML::BeginMap;
        {
            _out << YAML::Key << "m_IsVisible" << YAML::Value << m_RenderComponent->IsVisible();

            // Texture Settings
            _out << YAML::Newline << YAML::Comment("Texture Settings");
            _out << YAML::Key << "m_TextureSettings" << YAML::BeginMap;
            {
                TextureSettings texSet = m_RenderComponent->GetTextureSettings();
                _out << YAML::Key << "WrapMode" << YAML::Value << texSet.WrapMode;
                _out << YAML::Key << "WrapValue" << YAML::Value << texSet.WrapValue;
                _out << YAML::Key << "FilterMode" << YAML::Value << texSet.FilterMode;
                _out << YAML::Key << "FilterValue" << YAML::Value << texSet.FilterValue;
            }
            _out << YAML::EndMap;
            // End Texture Settings

            // Material
            if (const Ref<Material> mat = m_RenderComponent->GetMaterial())
            {
                _out << YAML::Newline << YAML::Comment("Material");
                _out << YAML::Key << "m_Material" << YAML::Value << (mat->GetAsset() ? mat->GetAsset()->GetRelativePath() : "");
            }
        }
        _out << YAML::EndMap;
        // End Render Component
        
        // Transform Component
        m_TransformComponent->Serialize(_out);
        
        // Physics Component
        m_PhysicsComponent->Serialize(_out);
        
        // Mesh Component
        _out << YAML::Newline << YAML::Comment("Mesh Component");
        _out << YAML::Key << "m_MeshComponent" << YAML::BeginMap;
        {
            _out << YAML::Key << "m_Mesh" << YAML::Value << m_MeshComponent->GetModel()->GetAssetName(); // Temp until asset scraper built
        }
        _out << YAML::EndMap;
        // End Mesh Component
    }

    void Actor::Deserialize(const YAML::Node& _in)
    {
        // Object instantiation is done in the SceneSubsystem so we don't need to do it here
        BaseObject::Deserialize(_in);

        // Render Component
        if(const YAML::Node& renderCompNode = _in["m_RenderComponent"]; renderCompNode)
        {
            m_RenderComponent->SetIsVisible(renderCompNode["m_IsVisible"].as<bool>());

            // Texture Settings
            if (const YAML::Node& texSettings = renderCompNode["m_TextureSettings"]; texSettings)
            {
                TextureSettings texSet;
                texSet.WrapMode = texSettings["WrapMode"].as<int>();
                texSet.WrapValue = texSettings["WrapValue"].as<int>();
                texSet.FilterMode = texSettings["FilterMode"].as<int>();
                texSet.FilterValue = texSettings["FilterValue"].as<int>();
                m_RenderComponent->SetTextureSettings(texSet);
            }

            // Material
            if (const YAML::Node matNode = renderCompNode["m_Material"]; !matNode.IsDefined())
            {
                m_RenderComponent->SetMaterial(AssetSubsystem::GetMaterial(matNode["m_Material"].as<std::string>()));
            }
        }

        // Transform Component
        if (_in["TransformComponent"].IsDefined()) m_TransformComponent->Deserialize(_in["TransformComponent"]);
        if (_in["PhysicsComponent"].IsDefined()) m_PhysicsComponent->Deserialize(_in["PhysicsComponent"]);

        // Mesh Component
        if (const YAML::Node meshComp = _in["m_MeshComponent"]; meshComp)
        {
            if (const Ref<Model> model = AssetSubsystem::GetModel(meshComp["m_Mesh"].as<std::string>()))
            {
                m_MeshComponent->SetModel(model); // Temp until asset scraper built
            }
            
        }
    }

    Ref<Collider> Actor::GetCollider() const
    { return m_PhysicsComponent->GetCollider(); }


    void Actor::Destroy()
    {
        // Add more clean up code here
        MarkRubbish();
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
}
