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
        m_Forward.x = cos(glm::radians(m_Transform.Rotation.y)) * cos(glm::radians(m_Transform.Rotation.x));
        m_Forward.y = sin(glm::radians(m_Transform.Rotation.x));
        m_Forward.z = sin(glm::radians(m_Transform.Rotation.y)) * cos(glm::radians(m_Transform.Rotation.x));
        m_Forward = normalize(m_Forward);

        m_Right = normalize(cross(m_Forward, {0.0f, 1.0f, 0.0f})); // World up
        m_Up = normalize(cross(m_Right, m_Forward));

        CalculateModel(m_Transform, m_Model);
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
