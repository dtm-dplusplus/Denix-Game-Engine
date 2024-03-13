#include "Denix/DePch.h"
#include "PhysicsSubSystem.h"

namespace Denix
{
	PhysicsSubSystem* PhysicsSubSystem::s_PhysicsSubSystem{ nullptr };

	void PhysicsSubSystem::Update(float _deltaTime)
	{
		for (const auto& component : m_PhysicsComponents)
		{
			if(!component->m_IsSimulated) continue;

			// Clear forces
			component->m_Force = glm::vec3(0.f);

			// Compute forces
			component->m_Force = component->m_Mass * component->m_Gravity;

			// Compute collision response
			if (component->m_TempPosition.y <= 0.f) // Temp 
			{
				component->m_TempPosition.y = 0.f;
				component->m_Velocity = glm::vec3(0.f);
			}

			// Step Intergration
			component->Step(_deltaTime);
		}
	}
}
