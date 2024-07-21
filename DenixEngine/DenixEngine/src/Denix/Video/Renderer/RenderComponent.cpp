#include "RenderComponent.h"

#include "Denix/Resource/ResourceSubsystem.h"


namespace Denix
{

	RenderComponent::RenderComponent(): Component(ObjectInitializer("Render Component"))
	{
		m_Shader = ResourceSubsystem::GetShader("DefaultShader");
		m_Texture = ResourceSubsystem::GetTexture("DefaultTexture");
		m_Material = ResourceSubsystem::GetMaterial("MAT_Default");
	}

	RenderComponent::RenderComponent(Ref<RenderComponent> _other)
	{
		m_Shader = _other->m_Shader;
		m_Texture = _other->m_Texture;
		m_Material = _other->m_Material;
		m_TextureSettings = _other->m_TextureSettings;
		m_DebugColor = _other->m_DebugColor;
		m_IsVisible = _other->m_IsVisible;
		m_AffectsLighting = _other->m_AffectsLighting;
	}

	RenderComponent::RenderComponent(const std::string& _parentName): Component(_parentName, ObjectInitializer("Render Component"))
	{
		m_Shader = ResourceSubsystem::GetShader("DefaultShader");
		m_Texture = ResourceSubsystem::GetTexture("DefaultTexture");
		m_Material = ResourceSubsystem::GetMaterial("MAT_Default");
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
