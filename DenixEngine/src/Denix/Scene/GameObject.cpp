#include "GameObject.h"

#include "Denix/Resource/ResourceSubsystem.h"

#include "yaml-cpp/yaml.h"

namespace YAML {
    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const ::YAML::Node& node, glm::vec3& rhs) {
            if(!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };
}

YAML::Node EmitVec3(const glm::vec3& vec) {
    YAML::Node node;
    node.push_back(vec.x);
    node.push_back(vec.y);
    node.push_back(vec.z);
    return node;
}

namespace Denix
{
   
    
    GameObject::GameObject(const ObjectInitializer& _object_init) : Object(_object_init)
    {
        m_TransformComponent = MakeRef<TransformComponent>(GetName());
        m_Components["Transform"] = m_TransformComponent;

        m_MeshComponent = MakeRef<MeshComponent>(GetName());
        m_Components["Mesh"] = m_MeshComponent;

        m_RenderComponent = MakeRef<RenderComponent>(GetName());
        m_RenderComponent->SetMaterial(MakeRef<Material>());
        m_Components["Render"] = m_RenderComponent;

        m_PhysicsComponent = MakeRef<PhysicsComponent>(GetName());
        m_PhysicsComponent->m_ParentTransform = m_TransformComponent;
        m_Components["Physics"] = m_PhysicsComponent;
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
            if (Ref<Material> mat = m_RenderComponent->GetMaterial())
            {
                _out << YAML::Key << "m_Material" << YAML::BeginMap;
                _out << YAML::Key << "m_SpecularIntensity" << YAML::Value << mat->GetSpecularIntensity();
                _out << YAML::Key << "m_SpecularPower" << YAML::Value << mat->GetSpecularPower();

                if(mat->GetShader()) // Temp until asset scraper built
                    _out << YAML::Key << "m_Shader" << YAML::Value << mat->GetShader()->GetFriendlyName();
                
                // Base Param
                _out << YAML::Key << "m_BaseParam" << YAML::Value << YAML::BeginMap;
                _out << YAML::Key << "Color" << YAML::Value << EmitVec3(mat->GetBaseParam().Color);
                _out << YAML::Key << "IsTexture" << YAML::Value << mat->GetBaseParam().IsTexture;
                if(mat->GetBaseParam().Texture) // Temp until asset scraper built
                    _out << YAML::Key << "Texture" << YAML::Value << mat->GetBaseParam().Texture->GetTextureName();
                _out << YAML::EndMap;
                _out << YAML::EndMap;
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
            _out << YAML::Key << "m_Scale" << YAML::Value << EmitVec3(m_TransformComponent->GetScale());
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
        if(const YAML::Node renderComp = _in["m_RenderComponent"]; renderComp)
        {
            m_RenderComponent->SetIsVisible(renderComp["m_IsVisible"].as<bool>());
            m_RenderComponent->SetAffectsLighting(renderComp["m_AffectsLighting"].as<bool>());

            // Texture Settings
            if (const YAML::Node texSettings = renderComp["m_TextureSettings"]; texSettings)
            {
                TextureSettings texSet;
                texSet.WrapMode = texSettings["WrapMode"].as<int>();
                texSet.WrapValue = texSettings["WrapValue"].as<int>();
                texSet.FilterMode = texSettings["FilterMode"].as<int>();
                texSet.FilterValue = texSettings["FilterValue"].as<int>();
                m_RenderComponent->SetTextureSettings(texSet);
            }

            if (const YAML::Node material = renderComp["m_Material"]; material)
            {
                Ref<Material> mat = MakeRef<Material>();
                mat->SetSpecularIntensity(material["m_SpecularIntensity"].as<float>());
                mat->SetSpecularPower(material["m_SpecularPower"].as<float>());

                if (const Ref<Shader> shader = ResourceSubsystem::GetShader(material["m_Shader"].as<std::string>()))
                {
                    mat->SetShader(shader); // Temp until asset scraper built
                }

                if (const YAML::Node baseParam = material["m_BaseParam"]; baseParam)
                {
                    BaseMatParam base;
                    base.Color = baseParam["Color"].as<glm::vec3>();
                    base.IsTexture = baseParam["IsTexture"].as<bool>();
                    if(baseParam["Texture"])
                    {
                        if (const Ref<Texture> tex = ResourceSubsystem::GetTexture(baseParam["Texture"].as<std::string>()))
                        {
                            base.Texture = tex; // Temp until asset scraper built
                        }
                    }
                   
                    mat->SetBaseParam(base);
                }

                m_RenderComponent->SetMaterial(mat);
            }
        }

        // Material
        
        
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
}
