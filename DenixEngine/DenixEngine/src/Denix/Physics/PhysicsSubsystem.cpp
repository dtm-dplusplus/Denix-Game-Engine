#include "Denix/depch.h"
#include "PhysicsSubsystem.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Scene/SceneSubsystem.h"

namespace Denix
{
	PhysicsSubsystem* PhysicsSubsystem::s_PhysicsSubSystem{ nullptr };

	void PhysicsSubsystem::RegisterComponent(const Ref<PhysicsComponent>& _component)
	{
		DE_LOG(LogPhysics, Trace, "PhysicsComponent Registered: #{} {}", _component->GetID(), _component->GetName())

			// Check if the component wants the scene gravity value
			if (!_component->m_IsCustomGravity) _component->m_Gravity = m_ActiveScene->GetGravity();

		m_PhysicsComponents.push_back(_component);
	}

	void PhysicsSubsystem::UnregisterComponent(const Ref<PhysicsComponent>& _component)
	{
		DE_LOG(LogPhysics, Trace, "PhysicsComponent Unregistered: #{} {}", _component->GetID(), _component->GetName())

			std::erase(m_PhysicsComponents, _component);
	}

	void PhysicsSubsystem::UpdateCollisionDetection(float _deltaTime)
	{
		for (const auto& component : m_PhysicsComponents)
		{
			// Skip if not simulated or level is not playing
			if (!component->m_IsSimulated || !m_IsSimulating) continue;

			// If static object no need to compute next step or sweep
			// Most objects are like wall are static so we check this first
			if (component->m_Moveability == Moveability::Static)
			{
				// If we detect a collision. Call collision event on game object

				continue;
			}

			// If dynamic object compute next step or sweep

		}
	}

	void PhysicsSubsystem::Update(float _deltaTime)
	{
		// Update collision detection
		UpdateCollisionDetection(_deltaTime);

		// Update physics component
		for (const auto& physicsComp : m_PhysicsComponents)
		{
			const Ref<ColliderComponent> colliderComp = physicsComp->m_ColliderComponent;

			// Skip if the scene is not simulating physics
			if (!m_IsSimulating) continue;

			// Skip if static object.  Only dynamic objects are simulated
			if(physicsComp->m_Moveability == Moveability::Static) continue;

			// Skip if not simulated
			if (!physicsComp->m_IsSimulated) continue;

			// From this point we assume the object is dynamic
			// Clear force & Compute mg
			physicsComp->m_Force = physicsComp->m_Mass * physicsComp->m_Gravity;

			// Compute collision response - Null effect for now
			if (physicsComp->m_ActorTransform->GetPosition().y <= 0.f) // Temp 
			{
				physicsComp->m_ActorTransform->GetPosition().y = 0.f;
				physicsComp->m_Velocity.y = 0.f;

				colliderComp->m_IsColliding = true;
				glm::vec4& color = colliderComp->m_RenderComponent->GetDebugColor();
				color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				colliderComp->m_IsColliding = false;
				glm::vec4& color = colliderComp->m_RenderComponent->GetDebugColor();
				color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			}

			// Step Intergration
			physicsComp->Step(_deltaTime);

			// Apply Constraints
		}
	}

	void PhysicsSubsystem::LateUpdate(float _deltaTime)
	{
		for (const auto& component : m_PhysicsComponents)
		{
			// Check if the component wants the scene gravity value
			if (component->m_RequestSceneGravity)
			{
				component->m_Gravity = m_ActiveScene->GetGravity();
				component->m_RequestSceneGravity = false;
			}
		}
	}

	
}
