#include "Material.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Core/Reflection/YAMLHelper.h"
#include "Denix/Asset/Asset.h"
#include "Denix/Core/File/FileSubsystem.h"

namespace Denix
{
	Material::	Material(const ObjectInit& _objInit) : BaseObject({_objInit})
	{
		m_Shader = AssetSubsystem::GetShader("DefaultShader");
	}

	Material::Material(const Ref<Material>& _other)
	{
		m_Shader = _other->m_Shader;
		m_BaseColor = _other->m_BaseColor;
		m_BaseTexture = _other->m_BaseTexture;
		CheckBaseType();
	}

	void Material::Serialize(YAML::Emitter& _out)
	{
		_out << YAML::Key << "m_BaseColor" << YAML::BeginMap;
		Vec3ToYAML(_out, m_BaseColor);
		_out << YAML::EndMap;
		
		_out << YAML::Key << "m_BaseTexture" << YAML::Value << (m_BaseTexture? m_BaseTexture->GetRelativePath() : "");

		_out << YAML::Key << "m_TextureSettings" << YAML::BeginMap;
		_out << YAML::Key << "WrapMode" << YAML::Value << m_TextureSettings.WrapMode;
		_out << YAML::Key << "WrapValue" << YAML::Value << m_TextureSettings.WrapValue;
		_out << YAML::Key << "FilterMode" << YAML::Value << m_TextureSettings.FilterMode;
		_out << YAML::Key << "FilterValue" << YAML::Value << m_TextureSettings.FilterValue;
		_out << YAML::EndMap;
	}

	void Material::Deserialize(const YAML::Node& _in)
	{
		if (const YAML::Node& baseColor = _in["m_BaseColor"]; baseColor.IsDefined()) m_BaseColor =YAMLtoVec3(baseColor);
        	
        		
		if (const YAML::Node& baseTex = _in["m_IsBaseTexture"]; baseTex.IsDefined())
			if (const Ref<Texture> tex = AssetSubsystem::GetTexture(baseTex.as<std::string>())) m_BaseTexture = tex;
			
		if (const YAML::Node& texSettings = _in["m_TextureSettings"]; texSettings.IsDefined())
		{
			if (const YAML::Node& wrapMode = texSettings["WrapMode"]; wrapMode.IsDefined())
				m_TextureSettings.WrapMode = wrapMode.as<int>();

			if (const YAML::Node& wrapValue = texSettings["WrapValue"]; wrapValue.IsDefined())
				m_TextureSettings.WrapValue = wrapValue.as<int>();

			if (const YAML::Node& filterMode = texSettings["FilterMode"]; filterMode.IsDefined())
				m_TextureSettings.FilterMode = filterMode.as<int>();

			if (const YAML::Node& filterValue = texSettings["FilterValue"]; filterValue.IsDefined())
				m_TextureSettings.FilterValue = filterValue.as<int>();
		}

		CheckBaseType();
	}

	void Material::SetBaseTexture(const Ref<Texture>& _texture)
	{
		// Check if the texture is valid
		if(!_texture)
		{
			DE_LOG(LogAsset, Error, "Texture is not valid")
		}

		m_BaseTexture = _texture;
		m_IsBaseTexture = true;
	}
}
