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

		// Clean collision colData
		m_CollisionEvents.clear();
		
		for (const auto& physicsComp : m_PhysicsComponents)
		{
			// Set  status
			physicsComp->m_SteppedThisFrame = physicsComp->m_SteppedNextFrame;
			physicsComp->m_SteppedNextFrame = false;
			physicsComp->m_IsColliding = false;

			physicsComp->m_Force = physicsComp->m_SimulateGravity? 
				glm::vec3(0.0f, physicsComp->m_Mass * -m_ActiveScene->GetGravity(), 0.0f) : glm::vec3(0.0f);

			physicsComp->m_Torque = glm::vec3(0.0f);


			physicsComp->m_PreviousPosition = physicsComp->m_ParentTransform->GetPosition();

			physicsComp->m_CenterOfMass = physicsComp->m_ParentTransform->GetPosition();
		}
	}

	void PhysicsSubsystem::Update(float _deltaTime)
	{
		if (!m_Enabled) return;
		if (!m_ActiveScene->IsPlaying()) return;
		
		if(m_CollisionDetectionEnabled) CollisionDetection(_deltaTime);

		if (m_CollisionResponseEnabled) CollisionResonsePhase(_deltaTime);

		PhysicsSimulationPhase(_deltaTime);
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

	void PhysicsSubsystem::CollisionResonsePhase(float _deltaTime)
	{
		for (CollisionEvent& collisionEvent : m_CollisionEvents)
		{
			if (!collisionEvent.Actor || !collisionEvent.Other) continue;
			CollisionResponse(collisionEvent);
		}
	}

	void PhysicsSubsystem::CollisionResponse(CollisionEvent& _collisionEvent)
	{
		Ref<PhysicsComponent> compActor  = _collisionEvent.Actor->GetPhysicsComponent();
		Ref<PhysicsComponent> compOther = _collisionEvent.Other->GetPhysicsComponent();

		// Impulse response
		glm::vec3 otherVelocity = compOther->GetVelocity();

		glm::vec3 contactForce = -glm::vec3(0.0f, compActor->m_Mass * -m_ActiveScene->GetGravity(), 0.0f);

		if (_collisionEvent.Actor->GetFriendlyName().find("Character") != std::string::npos)
		{
           DE_LOG(LogPhysics, Info, "Collision with Character")
        }

		if (compActor->m_ImpulseEnabled)
		{
			float impulseEnergy = ImpulseEnergy(
				compActor, compOther, _collisionEvent.ColData.Normal, _collisionEvent.ColData.ContactPoint);

			glm::vec3 impulseVector = impulseEnergy * _collisionEvent.ColData.Normal;

			compActor->m_Force = glm::vec3(0.0f);
			compActor->m_Velocity = impulseVector / compActor->m_Mass;

		}

		compActor->AddForce(contactForce);

		// Update collision status for rendering
		compActor->m_IsColliding = true;
		compOther->m_IsColliding = true;

		// Call client side implementation
		if(compActor->GetParentTransform()->GetMoveability() == Moveability::Dynamic)
			_collisionEvent.Actor->OnCollision(_collisionEvent.Other, _collisionEvent.ColData);

		if (compOther->GetParentTransform()->GetMoveability() == Moveability::Dynamic)
			_collisionEvent.Other->OnCollision(_collisionEvent.Actor, _collisionEvent.ColData);
	}

	void PhysicsSubsystem::SphereCubeCollision(const Ref<PhysicsComponent>& _compA, const Ref<PhysicsComponent>& _compB, Ref<SphereCollider>& _sphereColA, Ref<CubeCollider>& _cubeColB, CollisionEvent& _collisionEvent)
	{

	}

	void PhysicsSubsystem::PhysicsSimulationPhase(float _deltaTime)
	{
		for (const auto& physicsComp : m_PhysicsComponents)
		{
			if (!physicsComp->m_SimulatePhysics) continue;

			if (!physicsComp->m_SteppedThisFrame) physicsComp->StepSimulation(_deltaTime);
		}
	}
	float PhysicsSubsystem::ImpulseEnergy(const Ref<PhysicsComponent>& _compA, const Ref<PhysicsComponent>& _compB, const glm::vec3& _normal, const glm::vec3& _contactPoint)
	{
		float impulseEnergy = -(1.0f + _compA->m_Elasticity) *
			glm::dot(_compA->m_Velocity - _compB->m_Velocity, _normal) / (1.0f / _compB->m_Mass);

		return impulseEnergy;
	}
}