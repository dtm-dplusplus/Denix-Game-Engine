#include "BaseObject.h"
#include <yaml-cpp/yaml.h>

void Denix::BaseObject::Serialize(YAML::Emitter& _out)
{
    // Object Data. We can serialize the object data here without the need for reflection.
    _out << YAML::Key << "m_Object" << YAML::BeginMap;
    {
        _out << YAML::Key << "m_GUID" << YAML::Value << GetGUID();
        _out << YAML::Key << "m_Name" << YAML::Value << GetName();
        _out << YAML::Key << "m_ClassName" << YAML::Value << GetClassNameDE();
    }
    _out << YAML::EndMap;
}

void Denix::BaseObject::Deserialize(const YAML::Node& _in)
{
    const auto objNode = _in["m_Object"];
    if (!objNode.IsDefined())
    {
        DE_LOG(LogScene, Error, "No object data found")
        return;
    }

    // Class Name has already been set in the constructor so we don't need to set it here
    
    if (const auto guidNode = objNode["m_GUID"]; guidNode.IsDefined())
    {
        m_GUID = guidNode.as<size_t>();
    }
    else
    {
        DE_LOG(LogScene, Warn, "No object GUID found. Using default GUID")
    }
				
    // Set the object name
    if(const auto nameNode = objNode["m_Name"]; nameNode.IsDefined())
    {
        m_Name = nameNode.as<std::string>();
    }
    else
    {
        DE_LOG(LogScene, Warn, "No object name found. Using default name")
    }
}
