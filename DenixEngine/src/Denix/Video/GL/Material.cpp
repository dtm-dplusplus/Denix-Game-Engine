#include "Material.h"
#include "Denix/Resource/ResourceSubsystem.h"

namespace Denix
{
	Material::Material(const ObjectInitializer& _objInit) : Object({_objInit}), AO(0), Metallic(0), Roughness(0)
	{
		m_Shader = ResourceSubsystem::GetShader("DefaultShader");
	}

	Material::Material(Ref<Material> _other)
	{
		m_Shader = _other->m_Shader;
		BaseColor = _other->BaseColor;
		BaseTexture = _other->BaseTexture;
		CheckBaseType();
		m_SpecularIntensity = _other->m_SpecularIntensity;
		m_SpecularPower = _other->m_SpecularPower;
	}
}