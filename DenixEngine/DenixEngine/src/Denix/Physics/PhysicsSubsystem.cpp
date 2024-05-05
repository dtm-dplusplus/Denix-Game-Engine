#include "PhysicsSubsystem.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Physics/PhysicsComponent.h"
#include "Denix/Physics/Collider.h"

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
		for (const auto& physicsComp : m_PhysicsComponents)
		{
			Ref<Collider> colliderComp = physicsComp->m_Collider;
			bool collisionDetected{ false };

			// If static object no need to compute next step or sweep
			// Most objects are like wall are static so we check this first
			if (physicsComp->m_ActorTransform->m_Moveability == static_cast<int>(Moveability::Static))
			{
				// If we detect a collision. Call collision event on game object
				continue;
			}


			// Fake collision check for now
			if (physicsComp->m_ActorTransform->GetPosition().y <= 0.f)
			{
				collisionDetected = true;
			}

			// If Collider is trigger
			if (physicsComp->IsTrigger())
			{
				// Check Trigger State
				if (collisionDetected)
				{
					// Call OnTriggerStay
					if (physicsComp->m_IsColliding)
					{
						physicsComp->m_TriggerState = TriggerState::Stay;
					}
					// Call OnTriggerEnter
					else
					{
						physicsComp->m_TriggerState = TriggerState::Enter;
						//colliderComp->m_IsColliding = true;
					}
				}
				else
				{
					// Call OnTriggerExit
					if (physicsComp->m_IsColliding)
					{
						
						physicsComp->m_TriggerState = TriggerState::Exit;
						//colliderComp->m_IsColliding = false;
					}
					// Call OnTriggerNone
					else
					{
						physicsComp->m_TriggerState = TriggerState::Null;
					}
				}

				if (const Ref<GameObject> parentObj = m_ActiveScene->GetGameObject(physicsComp->m_ParentObjectName))
				{
					switch (physicsComp->m_TriggerState)
					{
					case TriggerState::Stay:
					{
						parentObj->OnTriggerStay();
					} break;

					case TriggerState::Enter:
					{
						parentObj->OnTriggerEnter();
					} break;

					case TriggerState::Exit:
					{
						parentObj->OnTriggerExit();
					} break;
					default: ;
					}
				}
			}

			colliderComp->m_RenderComponent->GetMaterial()->SetBaseColor(
				physicsComp->m_IsColliding ? colliderComp->m_CollisionColor : colliderComp->m_NoCollisionColor);

			physicsComp->m_IsColliding = collisionDetected;



		
		
		}
	}

	void PhysicsSubsystem::UpdatePhysicsComponents(float _deltaTime)
	{
		for (const auto& physicsComp : m_PhysicsComponents)
		{
			// Skip if not simulated
			if (!physicsComp->m_IsSimulated) continue;


			// Dynamic objects only
			//if (physicsComp->m_ParentMoveability == Moveability::Static) continue;
			
			// Compute collision response - Null effect for now


			// Step Intergration
			physicsComp->Step(_deltaTime);

			// Apply Constraints



			// Simulation loop :
			// 	9) compute the net force F(t);
			// 10) compute the net torque t(t) = sum of torque arm(x, (t)− x(t)) cross_product(f.) 11) Use Euler integration to :
			// update the velocity of COM : v(t + 1) = v(t) + F / m * dt; update the position of COM : x(t + 1) = x(t) + v(t + 1) * dt; update the angular momentum : L(t + 1) = L(t) + T * dt; compute the inverse inertia tensor I1 = R Ibody 1RT; update the angular velocity wt + 1 = | -1 Lt + 1
			// 	reconstruct the skew matrix w(*) (refer to my lecture note)
			// 	update the rotation matrix R of the rigid body : R(t + 1) = R(t) + dt w(*)R(t);
			// 12) apply the rotation to the rigid body using glm matrix for visualisation;
			// End of the simulation loop :
		}
	}

	void PhysicsSubsystem::BeginUpdate(float _deltaTime)
	{
		for (const auto& physicsComp : m_PhysicsComponents)
		{
			// Skip if not simulated
			if (!physicsComp->m_IsSimulated) continue;

			// Clear force & Compute mg
			physicsComp->m_Force = physicsComp->m_Mass * physicsComp->m_Gravity;
		}
	}

	void PhysicsSubsystem::Update(float _deltaTime)
	{
		if (!m_Enabled || !m_ActiveScene->IsPlaying()) return;

		UpdateCollisionDetection(_deltaTime);
		
		UpdatePhysicsComponents(_deltaTime);
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
