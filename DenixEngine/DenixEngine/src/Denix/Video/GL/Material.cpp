#include "Material.h"
#include "Denix/Resource/ResourceSubsystem.h"

namespace Denix
{
	Material::Material(const ObjectInitializer& _objInit) : Object({ _objInit })
	{
		m_Shader = ResourceSubsystem::GetShader("DefaultShader");
		m_BaseParam = BaseMatParam(glm::vec3(1.0f));
		m_NormalParam = NormalMatParam();
	}
	Material::Material(Ref<Material> _other)
	{
		m_Shader = _other->m_Shader;
		m_BaseParam = _other->m_BaseParam;
		m_NormalParam = _other->m_NormalParam;
		m_SpecularIntensity = _other->m_SpecularIntensity;
		m_SpecularPower = _other->m_SpecularPower;
	}
}