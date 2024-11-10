#include "GameObject.h"

#include "Denix/Resource/ResourceSubsystem.h"

#include "yaml-cpp/yaml.h"
#include "Denix/Core/YAMLHelper.h"
#include "Denix/Resource/Asset.h"

namespace Denix
{
    GameObject::GameObject()
    {
        m_TransformComponent = AddComponent<TransformComponent>();
        m_MeshComponent = AddComponent<MeshComponent>();
        m_RenderComponent = AddComponent<RenderComponent>();
        m_PhysicsComponent = AddComponent<PhysicsComponent>(m_TransformComponent);
    }

    GameObject::GameObject(const ObjectInitializer& _object_init) : BaseObject(_object_init)
    {
        m_TransformComponent = AddComponent<TransformComponent>();
        m_MeshComponent = AddComponent<MeshComponent>();
        m_RenderComponent = AddComponent<RenderComponent>();
        m_PhysicsComponent = AddComponent<PhysicsComponent>(m_TransformComponent);
    }

    void GameObject::Serialize(YAML::Emitter& _out)
    {
        // Object Data
        _out << YAML::Comment("Object Data");
        _out << YAML::Key << "m_Object" << YAML::BeginMap;
        {
            _out << YAML::Key << "m_Name" << YAML::Value << GetName();
            _out << YAML::Key << "m_FriendlyName" << YAML::Value << GetFriendlyName();
        }
        _out << YAML::EndMap;
        // End Object Data
        
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
                _out << YAML::Key << "m_Material" << YAML::Value << (mat->GetAsset() ? mat->GetAsset()->GetAssetPath() : "");

                // Save Material - Should be done in the editor.
                YAML::Emitter matAsssetEmitter;
                mat->Serialize(matAsssetEmitter);
                FileSubsystem::WriteFile(mat->GetAsset()->GetAssetPath(), matAsssetEmitter.c_str());
                DE_LOG(LogScene, Info, "Saved Material: {}", mat->GetAsset()->GetAssetFileName())
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
            _out << YAML::Key << "m_SimulateGravity" << YAML::Value << m_PhysicsComponent->GetSimulateGravity();
            
            _out << YAML::Key << "m_Collider" << YAML::Value <<  "Cube"; // Temp until asset scraper built
            
            _out << YAML::Key << "m_ColliderVisible" << YAML::Value << m_PhysicsComponent->IsColliderVisible();
            _out << YAML::Key << "m_CollisionDetectionEnabled" << YAML::Value << m_PhysicsComponent->CollisionDetectionEnabled();
            _out << YAML::Key << "m_CollisonDimesionOverride" << YAML::Value << m_PhysicsComponent->CollisionDimensionOverride();
            _out << YAML::Key << "m_IsTrigger" << YAML::Value << m_PhysicsComponent->IsTrigger();
            _out << YAML::Key << "m_ImpulseEnabled" << YAML::Value << m_PhysicsComponent->GetImpulseEnabled();
            _out << YAML::Key << "m_StepMethod" << YAML::Value << static_cast<int>(m_PhysicsComponent->GetStepMethod());
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
            _out << YAML::Key << "m_Mesh" << YAML::Value << m_MeshComponent->GetModel()->GetFriendlyName(); // Temp until asset scraper built
        }
        _out << YAML::EndMap;
        // End Mesh Component
    }

    void GameObject::Deserialize(const YAML::Node& _in)
    {
        // Object Data
        if (const YAML::Node object = _in["m_Object"]; object)
        {
            m_Name = object["m_Name"].as<std::string>();
            m_FriendlyName = object["m_FriendlyName"].as<std::string>();
        }
        
        // Render Component
        if(const YAML::Node renderCompNode = _in["m_RenderComponent"]; renderCompNode)
        {
            m_RenderComponent->SetIsVisible(renderCompNode["m_IsVisible"].as<bool>());
            m_RenderComponent->SetAffectsLighting(renderCompNode["m_AffectsLighting"].as<bool>());

            // Texture Settings
            if (const YAML::Node texSettings = renderCompNode["m_TextureSettings"]; texSettings)
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
                m_RenderComponent->SetMaterial(ResourceSubsystem::GetMaterial(matNode["m_Material"].as<std::string>()));
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
            m_PhysicsComponent->SetSimulateGravity(physicsComp["m_SimulateGravity"].as<bool>());
            m_PhysicsComponent->m_Collider = MakeRef<CubeCollider>();
            m_PhysicsComponent->IsColliderVisible() = physicsComp["m_ColliderVisible"].as<bool>();
            m_PhysicsComponent->SetCollisionDetectionEnabled(physicsComp["m_CollisionDetectionEnabled"].as<bool>());
            m_PhysicsComponent->CollisionDimensionOverride() = physicsComp["m_CollisonDimesionOverride"].as<bool>();
            m_PhysicsComponent->IsTrigger() = physicsComp["m_IsTrigger"].as<bool>();
            m_PhysicsComponent->SetImpulseEnabled(physicsComp["m_ImpulseEnabled"].as<bool>());
            m_PhysicsComponent->SetStepMethod(static_cast<StepMethod>(physicsComp["m_StepMethod"].as<int>()));
            m_PhysicsComponent->GetMass() = physicsComp["m_Mass"].as<float>();
            m_PhysicsComponent->GetLinearDrag() = physicsComp["m_LinearDrag"].as<float>();
            m_PhysicsComponent->SetElasticity(physicsComp["m_Elasticity"].as<float>());
            m_PhysicsComponent->GetAngularDrag() = physicsComp["m_AngularDrag"].as<float>();
        }

        // Mesh Component
        if (const YAML::Node meshComp = _in["m_MeshComponent"]; meshComp)
        {
            if (const Ref<Model> model = ResourceSubsystem::GetModel(meshComp["m_Mesh"].as<std::string>()))
            {
                m_MeshComponent->SetModel(model); // Temp until asset scraper built
            }
            
        }
    }

    void GameObject::OnTriggerEnter(Ref<GameObject> _other)
    {}

    void GameObject::OnTriggerStay(Ref<GameObject> _other)
    {}

    void GameObject::OnTriggerExit(Ref<GameObject> _other)
    {}
}
