#include "RenderComponent.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Core/Reflection/YAMLHelper.h"

namespace Denix
{
    RenderComponent::RenderComponent(): Component(ObjectInit("Render Component"))
    {
        m_ClassName = "RenderComponent";
        m_Shader = AssetSubsystem::GetShader("DefaultShader");
        m_Material = MakeRef<Material>();
    }

    void RenderComponent::SetMaterial(const Ref<Material>& _material)
    {
        // Check if the material is valid
        if (!IsValid(_material))
        {
            DE_LOG(LogRender, Error, "Material is not valid")
            return;
        }

        m_Material = _material;
        m_Material->CheckBaseType();
    }

    void RenderComponent::Serialize(YAML::Emitter& _out)
    {
        Component::Serialize(_out);

        _out << YAML::Key << "m_IsVisible" << YAML::Value << m_IsVisible;
        _out << YAML::Key << "m_IsUI" << YAML::Value << m_IsVisible;

        _out << YAML::Key << "m_Material" << YAML::BeginMap;
        m_Material->Serialize(_out);
        _out << YAML::EndMap;
    }

    void RenderComponent::Deserialize(const YAML::Node& _in)
    {
        Component::Deserialize(_in);

        if (const YAML::Node& vis = _in["m_IsVisible"]; vis.IsDefined()) m_IsVisible = vis.as<bool>();
        if (const YAML::Node& ui = _in["m_IsUI"]; ui.IsDefined()) m_IsUI = ui.as<bool>();
        if (const YAML::Node& mat = _in["m_Material"]; mat.IsDefined()) m_Material->Deserialize(mat);
    }
}
