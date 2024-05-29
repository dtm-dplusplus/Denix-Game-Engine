#include "RenderComponent.h"

#include "Denix/Resource/ResourceSubsystem.h"


namespace Denix
{

	RenderComponent::RenderComponent(): Component(ObjectInitializer("Render Component"))
	{
		m_Material = MakeRef<DefaultMaterial>();
	}

	RenderComponent::RenderComponent(Ref<RenderComponent> _other)
	{
		m_Material = _other->m_Material;
		m_TextureSettings = _other->m_TextureSettings;
		m_IsVisible = _other->m_IsVisible;
		m_AffectsLighting = _other->m_AffectsLighting;
	}

	RenderComponent::RenderComponent(const std::string& _parentName): Component(_parentName, ObjectInitializer("Render Component"))
	{
		m_Material = MakeRef<DefaultMaterial>();
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
