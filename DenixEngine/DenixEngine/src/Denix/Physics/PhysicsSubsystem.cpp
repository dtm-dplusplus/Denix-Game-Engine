#include "depch.h"
#include "PhysicsSubsystem.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Physics/PhysicsComponent.h"
#include "Denix/Physics/Collider.h"

namespace Denix
{
	PhysicsSubsystem* PhysicsSubsystem::s_PhysicsSubSystem{ nullptr };

	void PhysicsSubsystem::RegisterComponent(const Ref<PhysicsComponent>& _component)
	{
		// DE_LOG(LogPhysics, Trace, "PhysicsComponent Registered: #{} {}", _component->GetID(), _component->GetName())
		m_PhysicsComponents.push_back(_component);
	}

	void PhysicsSubsystem::UnregisterComponent(const Ref<PhysicsComponent>& _component)
	{
		// (LogPhysics, Trace, "PhysicsComponent Unregistered: #{} {}", _component->GetID(), _component->GetName())
		std::erase(m_PhysicsComponents, _component);
	}

	void PhysicsSubsystem::PreUpdate(float _deltaTime)
	{
		Subsystem::PreUpdate(_deltaTime);

		if (!m_Enabled || !m_ActiveScene->IsPlaying()) return;

		for (const auto& physicsComp : m_PhysicsComponents)
		{
			// Clean collision colData
			m_CollisionEvents.clear();

			// Set  status
			physicsComp->m_SteppedThisFrame = physicsComp->m_SteppedNextFrame;
			physicsComp->m_SteppedNextFrame = false;
			physicsComp->m_IsColliding = false;

			physicsComp->m_Force = glm::vec3(0.0f, physicsComp->m_Mass * -m_ActiveScene->GetGravity(), 0.0f);

			//actor->m_Torque = glm::vec3(0.0f);

			physicsComp->m_PreviousPosition = physicsComp->m_ParentTransform->GetPosition();

			physicsComp->m_CenterOfMass = physicsComp->m_ParentTransform->GetPosition();
		}
	}

	void PhysicsSubsystem::Update(float _deltaTime)
	{
		if (!m_Enabled) return;

		if(m_CollisionDetectionEnabled && m_ActiveScene->IsPlaying()) CollisionDetection(_deltaTime);

		if (m_CollisionResponseEnabled && m_ActiveScene->IsPlaying()) CollisionResonse(_deltaTime);

		PhysicsSimulation(_deltaTime);
	}
 
	void PhysicsSubsystem::CollisionDetection(float _deltaTime)
	{
		// Sort physics components into static and dynamic objects - Check they have collision detection enabled
		std::vector<Ref<PhysicsComponent>> staticObjects;
		std::vector<Ref<PhysicsComponent>> dynamicObjects;

		for (const auto& comp : m_PhysicsComponents)
		{
			if (comp->m_ParentTransform->GetMoveability() == Moveability::Static)
			{
				if(comp->CollisionDetectionEnabled()) staticObjects.push_back(comp);
			}
			else
			{
				if (comp->CollisionDetectionEnabled()) dynamicObjects.push_back(comp);
			}
		}

		// Compute collision detection - Skip Broad phase for now
		for (const auto& dynamicComp : dynamicObjects)
		{
			// Check for collision with static objects
			for (const auto& staticComp : staticObjects)
			{
				if (CollisionDetection::BroadCollisionDetection(dynamicComp, staticComp); true)
				{
					if (CollisionEvent collision = CollisionDetection::NarrowCollisionDetection(dynamicComp, staticComp); collision.IsCollision)
					{
						m_CollisionEvents.push_back(collision);
					}
				}
			}

			// Check for collision with other dynamic objects
			for (const auto& otherDynamicComp : dynamicObjects)
			{
				if (dynamicComp == otherDynamicComp) continue;

				if (CollisionDetection::BroadCollisionDetection(dynamicComp, otherDynamicComp); true)
				{
					if (CollisionEvent collision = CollisionDetection::NarrowCollisionDetection(dynamicComp, otherDynamicComp); collision.IsCollision)
					{
						m_CollisionEvents.push_back(collision);
					}
				}

			}
		}
	}

	void PhysicsSubsystem::CollisionResonse(float _deltaTime)
	{
		for (auto& [isCollison, actor, other, colData] : m_CollisionEvents)
		{
			if (!actor || !other) continue;

			if (Ref<PhysicsComponent> comp = actor->GetPhysicsComponent())
			{
				// Impulse response
				/*glm::vec3 planeVelocity = glm::vec3(0.0f);

				glm::vec3 contactForce =  -comp->GetForce();
				glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

				if (comp->m_ImpulseEnabled)
				{
					float impulseEnergy = -(1.0f + comp->m_Elasticity) *
						glm::dot(comp->m_Velocity - planeVelocity, normal) / (1.0f / comp->m_Mass);

					glm::vec3 impulseVector = impulseEnergy * normal;

					comp->m_Force = glm::vec3(0.0f);
					comp->m_Velocity = impulseVector / comp->m_Mass;
				}*/
				// comp->AddForce(contactForce);

				comp->m_Force = glm::vec3(0.0f);
				comp->m_Velocity = glm::vec3(0.0f);

				// Call client side implementation
				actor->GetPhysicsComponent()->m_IsColliding = true;
				other->GetPhysicsComponent()->m_IsColliding = true;

				actor->OnCollision(other, colData);
				if (other) other->OnCollision(actor, colData);
			}
		}
	}

	void PhysicsSubsystem::PhysicsSimulation(float _deltaTime)
	{
		for (const auto& physicsComp : m_PhysicsComponents)
		{
			if (!physicsComp->m_SimulatePhysics) continue;

			// Compute Torque
			//actor->m_Torque = actor->m_Force * actor->m_Radius;

			// Step Integration - Collision detection may have already stepped for dynamic objects
			if (!physicsComp->m_SteppedThisFrame) physicsComp->StepSimulation(_deltaTime);


			// update the angular momentum : L(t + 1) = L(t) + T * dt;
			// actor->m_AngularMomentum += actor->m_Radius * actor->m_Mass  * actor->m_Velocity * _deltaTime;

			// compute the inverse inertia tensor I1 = R Ibody 1RT; 
			// actor->ComputeInverseInertiaTensor();

			// update the angular velocity wt + 1 = | -1 Lt + 1
			// 	reconstruct the skew matrix w(*) (refer to my lecture note)
			// 	update the rotation matrix R of the rigid body : R(t + 1) = R(t) + dt w(*)R(t);
			// 12) apply the rotation to the rigid body using glm matrix for visualisation;		
		}
	}
}