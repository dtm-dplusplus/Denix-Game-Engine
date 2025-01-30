#include "ModelComponent.h"

#include "Denix/Core/Reflection/YAMLHelper.h"

Denix::ModelComponent::ModelComponent(): Component(ObjectInit("Model Component")),
                                         m_Model{nullptr}
{
    m_ClassName = "ModelComponent";
}

void Denix::ModelComponent::Serialize(YAML::Emitter& _out)
{
    Component::Serialize(_out);

    _out << YAML::Key << "m_Model" << YAML::Value << (m_Model ? m_Model->GetRelativePath() : "");
}

void Denix::ModelComponent::Deserialize(const YAML::Node& _in)
{
    Component::Deserialize(_in);

    if (const YAML::Node model = _in["m_Model"]; model.IsDefined())
        m_Model = AssetSubsystem::GetModel(model.as<std::string>());
}
