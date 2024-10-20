#include "Material.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Core/YAMLHelper.h"
#include "Denix/Resource/Asset.h"

namespace Denix
{
	Material::Material(const ObjectInitializer& _objInit) : BaseObject({_objInit}), AO(0), Metallic(0), Roughness(0)
	{
		m_Shader = ResourceSubsystem::GetShader("DefaultShader");
	}

	Material::Material(const Ref<Material>& _other)
	{
		m_Shader = _other->m_Shader;
		m_BaseColor = _other->m_BaseColor;
		BaseTexture = _other->BaseTexture;
		CheckBaseType();
		m_SpecularIntensity = _other->m_SpecularIntensity;
		m_SpecularPower = _other->m_SpecularPower;
	}

	Material::Material(const Ref<Asset>& _asset): BaseObject({_asset->GetAssetName()})
	{
		m_Asset = _asset;
		Deserialize(_asset);
		CheckBaseType();
	}

	void Material::Serialize(YAML::Emitter& _out)
	{
		_out << YAML::Key << "m_Material" << YAML::BeginMap;
		_out << YAML::Key << "m_Asset" << YAML::Value << (m_Asset? m_Asset->GetAssetPath() : "");
		_out << YAML::Key << "m_BaseColor" << YAML::Value << EmitVec3(m_BaseColor);
		_out << YAML::Key << "m_BaseTexture" << YAML::Value << (BaseTexture? BaseTexture->GetTextureName() : "");
		_out << YAML::Key << "m_Shader" << YAML::Value << m_Shader->GetFriendlyName();
		_out << YAML::Key << "m_SpecularIntensity" << YAML::Value << m_SpecularIntensity;
		_out << YAML::Key << "m_SpecularPower" << YAML::Value << m_SpecularIntensity;
		_out << YAML::EndMap;
	}

	void Material::Deserialize(const YAML::Node& _in)
	{
		BaseObject::Deserialize(_in);

		SetSpecularIntensity(_in["m_SpecularIntensity"].as<float>());
		SetSpecularPower(_in["m_SpecularPower"].as<float>());

		if (const Ref<Shader> shader = ResourceSubsystem::GetShader(_in["m_Shader"].as<std::string>()))
		{
			SetShader(shader); // Temp until asset scraper built
		}

		SetBaseColor(_in["m_BaseColor"].as<glm::vec3>());
		//SetBaseColor(ResourceSubsystem::GetTexture(_in["m_BaseTexture"].as<std::string>()));    
		CheckBaseType();
	}

	YAML::Node Material::Deserialize(const Ref<Asset>& _asset)
	{
		YAML::Node matNode = YAML::LoadFile(_asset->GetAssetPath());

		if(matNode["m_Material"])
		{
			Deserialize(matNode["m_Material"]);
		}

		return matNode;
	}

	void Material::SetBaseTexture(const Ref<Texture>& _texture)
	{
		// Check if the texture is valid
		if(!_texture)
		{
			DE_LOG(LogResource, Error, "Texture is not valid");
		}

		BaseTexture = _texture;
		IsBaseTexture = true;
	}
}
