#include "RenderComponent.h"

#include "Denix/Resource/ResourceSubsystem.h"


namespace Denix
{

	RenderComponent::RenderComponent(): Component(ObjectInit("Render Component"))
	{
		m_Shader = ResourceSubsystem::GetShader("DefaultShader");
		m_Texture = ResourceSubsystem::GetTexture("DefaultTexture");
		m_Material = ResourceSubsystem::GetDefaultMaterial();
	}

	RenderComponent::RenderComponent(Ref<RenderComponent> _other)
	{
		m_Shader = _other->m_Shader;
		m_Texture = _other->m_Texture;
		m_Material = _other->m_Material;
		m_TextureSettings = _other->m_TextureSettings;
		m_IsVisible = _other->m_IsVisible;
		m_AffectsLighting = _other->m_AffectsLighting;
	}

	RenderComponent::RenderComponent(const std::string& _parentName): Component(_parentName, ObjectInit("Render Component"))
	{
		m_Shader = ResourceSubsystem::GetShader("DefaultShader");
		m_Texture = ResourceSubsystem::GetTexture("DefaultTexture");
		m_Material = ResourceSubsystem::GetDefaultMaterial();
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
	}

	void RenderComponent::BeginScene()
	{
		Component::BeginScene();

		//RegisterComponent();
	}

	void RenderComponent::EndScene()
	{
		//UnregisterComponent();

		Component::EndScene();
	}

	void RenderComponent::RegisterComponent()
	{
		// Component::RegisterComponent();
	}

	void RenderComponent::UnregisterComponent()
	{
		// Component::UnregisterComponent();
	}
}
