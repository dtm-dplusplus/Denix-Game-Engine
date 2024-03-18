#include "Denix/depch.h"
#include "PhysicsSubSystem.h"

namespace Denix
{
	PhysicsSubSystem* PhysicsSubSystem::s_PhysicsSubSystem{ nullptr };

	void PhysicsSubSystem::Update(float _deltaTime)
	{
		for (const auto& component : m_PhysicsComponents)
		{
			if(!component->m_IsSimulated) continue;

			// Clear force & Compute mg
			component->m_Force = component->m_Mass * component->m_Gravity;

			// Compute collision response - Null effect for now
			component->m_ColliderComponent->ComputeCollision();

			// Step Intergration
			component->Step(_deltaTime);

			if (component->m_ColliderComponent->m_IsColliding) component->m_ActorTransformComponent->GetPosition().y = 0.f;
		}
	}
}
