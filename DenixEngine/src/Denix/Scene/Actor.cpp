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
            _out << YAML::Key << "m_AffectsLighting" << YAML::Value << m_RenderComponent->AffectsLighting();

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
        _out << YAML::Newline << YAML::Comment("Transform Component");
        _out << YAML::Key << "m_TransformComponent" << YAML::BeginMap;
        {
            _out << YAML::Key << "m_Position" << YAML::Value << EmitVec3(m_TransformComponent->GetPosition());
            _out << YAML::Key << "m_Rotation" << YAML::Value << EmitVec3(m_TransformComponent->GetRotation());
            _out << YAML::Key << "m_Scale" << YAML::Value <<EmitVec3(m_TransformComponent->GetScale());
            _out << YAML::Key << "m_Moveability" << YAML::Value << static_cast<int>(m_TransformComponent->GetMoveability());
        }
        _out << YAML::EndMap;
        // End Transform Component
        
        // Physics Component
        _out << YAML::Newline << YAML::Comment("Physics Component");
        _out << YAML::Key << "m_PhysicsComponent" << YAML::BeginMap;
        {
            _out << YAML::Key << "m_SimulatePhysics" << YAML::Value << m_PhysicsComponent->SimulatePhysics();
            
            _out << YAML::Key << "m_Collider" << YAML::Value <<  "Cube"; // Temp until asset scraper built
            
            _out << YAML::Key << "m_CollisionDetectionEnabled" << YAML::Value << m_PhysicsComponent->CollisionDetectionEnabled();
            _out << YAML::Key << "m_CollisonDimesionOverride" << YAML::Value << m_PhysicsComponent->CollisionDimensionOverride();
            _out << YAML::Key << "m_ImpulseEnabled" << YAML::Value << m_PhysicsComponent->GetImpulseEnabled();
            _out << YAML::Key << "m_Mass" << YAML::Value << m_PhysicsComponent->GetMass();
            _out << YAML::Key << "m_LinearDrag" << YAML::Value << m_PhysicsComponent->GetLinearDrag();
            _out << YAML::Key << "m_Elasticity" << YAML::Value << m_PhysicsComponent->GetElasticity();
            _out << YAML::Key << "m_AngularDrag" << YAML::Value << m_PhysicsComponent->GetAngularDrag();
        }
        _out << YAML::EndMap;
        // End Physics Component
        
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
            m_RenderComponent->SetAffectsLighting(renderCompNode["m_AffectsLighting"].as<bool>());

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
        if (const YAML::Node transformComp = _in["m_TransformComponent"]; transformComp)
        {
            m_TransformComponent->SetPosition(transformComp["m_Position"].as<glm::vec3>());
            m_TransformComponent->SetRotation(transformComp["m_Rotation"].as<glm::vec3>());
            m_TransformComponent->SetScale(transformComp["m_Scale"].as<glm::vec3>());
            m_TransformComponent->SetMoveability(static_cast<Moveability>(transformComp["m_Moveability"].as<int>()));
        }
        

        // Physics Component
        if (const YAML::Node physicsComp = _in["m_PhysicsComponent"]; physicsComp)
        {
            m_PhysicsComponent->SetSimulatePhysics(physicsComp["m_SimulatePhysics"].as<bool>());
            m_PhysicsComponent->m_Collider = MakeRef<CubeCollider>();
            m_PhysicsComponent->SetCollisionDetectionEnabled(physicsComp["m_CollisionDetectionEnabled"].as<bool>());
            m_PhysicsComponent->CollisionDimensionOverride() = physicsComp["m_CollisonDimesionOverride"].as<bool>();
            m_PhysicsComponent->SetImpulseEnabled(physicsComp["m_ImpulseEnabled"].as<bool>());
            m_PhysicsComponent->GetMass() = physicsComp["m_Mass"].as<float>();
            m_PhysicsComponent->GetLinearDrag() = physicsComp["m_LinearDrag"].as<float>();
            m_PhysicsComponent->SetElasticity(physicsComp["m_Elasticity"].as<float>());
            m_PhysicsComponent->GetAngularDrag() = physicsComp["m_AngularDrag"].as<float>();
        }

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
    }
}
