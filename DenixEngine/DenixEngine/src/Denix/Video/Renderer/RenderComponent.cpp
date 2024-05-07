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
