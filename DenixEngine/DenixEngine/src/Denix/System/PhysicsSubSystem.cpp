#include "Denix/depch.h"
#include "PhysicsSubsystem.h"
#include "Denix/System/SceneSubsystem.h"

namespace Denix
{
	PhysicsSubsystem* PhysicsSubsystem::s_PhysicsSubSystem{ nullptr };

	void PhysicsSubsystem::Update(float _deltaTime)
	{
		for (const auto& component : m_PhysicsComponents)
		{
			// Check if the component wants the scene gravity value
			if (component->m_RequestSceneGravity)
			{
				if (const Ref<Scene> scene = SceneSubsystem::Get()->GetActiveScene())
				{
					component->m_Gravity = scene->GetGravity();
					component->m_RequestSceneGravity = false;
				}
			}
			// Compute center of mass
			// We only have one Rigidbody so total masss = m_Mass
			const float totalMass = component->m_Mass;
			glm::vec3 firstMoment = component->m_Mass * component->m_ActorTransformComponent->GetPosition();

			component->m_CenterOfMass = firstMoment / totalMass;

			// Skip if not simulated or level is not playing -> We compute the above values for debug purposes
			if (!component->m_IsSimulated || !m_IsSimulating) continue;

			// Clear force & Compute mg
			component->m_Force = component->m_Mass * component->m_Gravity;

			// Compute collision response - Null effect for now
			component->m_ColliderComponent->ComputeCollision();

			// Step Intergration
			component->Step(_deltaTime);

			// TEMP
			if (component->m_ColliderComponent->m_IsColliding)
			{
				component->m_ActorTransformComponent->GetPosition().y = 0.f;
				component->m_Velocity.y = 0.f;
			}
		}
	}
}
