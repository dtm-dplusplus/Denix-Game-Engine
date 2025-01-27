#include "TransformComponent.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Core/Reflection/YAMLHelper.h"

namespace Denix
{
    TransformComponent::TransformComponent(): Component(ObjectInit("Transform Component")),
                                              m_Forward({0.0f, 0.0f, -1.0f}), m_Right({1.0f, 0.0f, 0.0f}),
                                              m_Up({0.0f, 1.0f, 0.0f})
    
    {
        m_ClassName = "TransformComponent";
    }

    void TransformComponent::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        glm::vec3 fwd;
        fwd.x = cos(glm::radians(m_Transform.Rotation.y)) * cos(glm::radians(m_Transform.Rotation.x));
        fwd.y = sin(glm::radians(m_Transform.Rotation.x));
        fwd.z = sin(glm::radians(m_Transform.Rotation.y)) * cos(glm::radians(m_Transform.Rotation.x));
        m_Forward = glm::normalize(fwd);

        m_Right = glm::normalize(glm::cross(m_Forward, {0.0f, 1.0f, 0.0f})); // World up
        m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
        
        m_Model = glm::translate(glm::mat4(1.0f), m_Transform.Position);

        m_Model = glm::rotate(m_Model, glm::radians(m_Transform.Rotation.x), glm::vec3(1, 0, 0));
        m_Model = glm::rotate(m_Model, glm::radians(m_Transform.Rotation.y), glm::vec3(0, 1, 0));
        m_Model = glm::rotate(m_Model, glm::radians(m_Transform.Rotation.z), glm::vec3(0, 0, 1));
        
        // Scale Model by half to account for axis sign
        m_Model = glm::scale(m_Model, m_Transform.Scale / 2.0f);
    }

    void TransformComponent::UpdateRotationVectorFromMatrix()
    {
        // Update Rotation Vector
        m_Transform.Rotation.x = fmod(m_Transform.Rotation.x, 360.f);
        m_Transform.Rotation.y = fmod(m_Transform.Rotation.y, 360.f);
        m_Transform.Rotation.z = fmod(m_Transform.Rotation.z, 360.f);
    }

    void TransformComponent::Serialize(YAML::Emitter& _out)
    {
        Component::Serialize(_out);

        _out << YAML::Key << "m_Position" << YAML::BeginMap;
        Vec3ToYAML(_out, m_Transform.Position);
        _out << YAML::EndMap;
            
        _out << YAML::Key << "m_Rotation" << YAML::BeginMap;
        Vec3ToYAML(_out, m_Transform.Rotation);
        _out << YAML::EndMap;

        _out << YAML::Key << "m_Scale" << YAML::BeginMap;
        Vec3ToYAML(_out, m_Transform.Scale);
        _out << YAML::EndMap;
    }

    void TransformComponent::Deserialize(const YAML::Node& _in)
    {
        Component::Deserialize(_in);

        // Transform Component
        if (const YAML::Node& pos = _in["m_Position"]; pos.IsDefined()) m_Transform.Position = YAMLtoVec3(pos);
        if (const YAML::Node& rot = _in["m_Rotation"]; rot.IsDefined()) m_Transform.Rotation = YAMLtoVec3(rot);
        if (const YAML::Node& scale = _in["m_Scale"]; scale.IsDefined()) m_Transform.Scale = YAMLtoVec3(scale);
    }
}
