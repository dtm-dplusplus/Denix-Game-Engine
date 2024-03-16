#include "RenderComponent.h"

namespace Denix
{
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
		Component::RegisterComponent();
	}

	void RenderComponent::UnregisterComponent()
	{
		Component::UnregisterComponent();
	}
}