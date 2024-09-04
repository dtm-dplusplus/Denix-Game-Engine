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
        _out << YAML::BeginMap;
        _out << YAML::Key << "Class" << YAML::Value << "GameObject";
        _out << YAML::Key << "m_Name" << YAML::Value << GetName();
        _out << YAML::Key << "m_FriendlyName" << YAML::Value << GetFriendlyName();

        // Render Component
        _out << YAML::Newline << YAML::Comment("Render Component");
        _out << YAML::Key << "m_IsVisible" << YAML::Value << m_RenderComponent->IsVisible();
        _out << YAML::Key << "m_AffectsLighting" << YAML::Value << m_RenderComponent->AffectsLighting();

        // Transform Component
        _out << YAML::Newline << YAML::Comment("Transform Component");
        _out << YAML::Key << "m_Position" << YAML::Value << EmitVec3(m_TransformComponent->GetPosition());
        _out << YAML::Key << "m_Rotation" << YAML::Value << EmitVec3(m_TransformComponent->GetRotation());
        _out << YAML::Key << "m_Scale" << YAML::Value << EmitVec3(m_TransformComponent->GetScale());
        _out << YAML::Key << "m_Moveability" << YAML::Value << static_cast<int>(m_TransformComponent->GetMoveability());

        // Physics Component
        _out << YAML::Newline << YAML::Comment("Physics Component");
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
        _out << YAML::EndMap;

        YAML::Node n = YAML::LoadFile("object.yaml");
    }

    void GameObject::Deserialize(const YAML::Node& _in)
    {
        // Render Component
        m_RenderComponent->SetIsVisible(_in["m_IsVisible"].as<bool>());
        m_RenderComponent->SetAffectsLighting(_in["m_AffectsLighting"].as<bool>());

        // Transform Component
        m_TransformComponent->SetPosition(_in["m_Position"].as<glm::vec3>());
        m_TransformComponent->SetRotation(_in["m_Rotation"].as<glm::vec3>());
        m_TransformComponent->SetScale(_in["m_Scale"].as<glm::vec3>());
        m_TransformComponent->SetMoveability(static_cast<Moveability>(_in["m_Moveability"].as<int>()));

        // Physics Component
        m_PhysicsComponent->SetSimulatePhysics(_in["m_SimulatePhysics"].as<bool>());
        m_PhysicsComponent->SetSimulateGravity(_in["m_SimulateGravity"].as<bool>());
        m_PhysicsComponent->IsColliderVisible() = _in["m_ColliderVisible"].as<bool>();
        m_PhysicsComponent->SetCollisionDetectionEnabled(_in["m_CollisionDetectionEnabled"].as<bool>());
        m_PhysicsComponent->CollisionDimensionOverride() = _in["m_CollisonDimesionOverride"].as<bool>();
        m_PhysicsComponent->IsTrigger() = _in["m_IsTrigger"].as<bool>();
        m_PhysicsComponent->SetImpulseEnabled(_in["m_ImpulseEnabled"].as<bool>());
        m_PhysicsComponent->SetStepMethod(static_cast<StepMethod>(_in["m_StepMethod"].as<int>()));
        m_PhysicsComponent->GetMass() = _in["m_Mass"].as<float>();
        m_PhysicsComponent->GetLinearDrag() = _in["m_LinearDrag"].as<float>();
        m_PhysicsComponent->SetElasticity(_in["m_Elasticity"].as<float>());
        m_PhysicsComponent->GetAngularDrag() = _in["m_AngularDrag"].as<float>();
    }
}
