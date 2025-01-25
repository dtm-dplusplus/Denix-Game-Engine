#include "RenderComponent.h"

#include "Denix/Asset/AssetSubsystem.h"


namespace Denix
{

	RenderComponent::RenderComponent(): Component(ObjectInit("Render Component"))
	{
		m_Shader = AssetSubsystem::GetShader("DefaultShader");
		m_Texture = AssetSubsystem::GetTexture("DefaultTexture");
		m_Material = MakeRef<Material>(AssetSubsystem::GetDefaultMaterial());
	}

	void RenderComponent::SetMaterial(const Ref<Material>& _material)
	{
		// Check if the material is valid
		if (!IsValid(_material))
		{
			DE_LOG(LogRender, Error, "Material is not valid");
			return;
		}
		
		m_Material = _material;
		m_Material->CheckBaseType();
	}
}
