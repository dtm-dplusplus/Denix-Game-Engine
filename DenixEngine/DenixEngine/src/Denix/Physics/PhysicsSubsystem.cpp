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
						if(!ColllisionExists(collision.Actor, collision.Other)) m_CollisionEvents.push_back(collision);
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
						if (!ColllisionExists(collision.Actor, collision.Other)) m_CollisionEvents.push_back(collision);
					}
				}

			}
		}
	}

	bool PhysicsSubsystem::ColllisionExists(const Ref<GameObject>& _objectA, const Ref<GameObject>& _objectB)
	{
		for (const auto& col : m_CollisionEvents)
		{
			if (col.Actor == _objectA && col.Other == _objectB ||
				col.Actor == _objectB && col.Other == _objectA) return true;
		}

		return false;
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

		// Collider based response
		switch (compActor->GetCollider()->GetColliderType())
		{
		case ColliderType::Cube:
		{
			switch (compOther->GetCollider()->GetColliderType())
			{
			case ColliderType::Cube:
			{
				CubeCollision(compActor, compOther, _collisionEvent);

			} break;

			case ColliderType::Sphere:
			{
				// Cube to Sphere Collision Detection
				SphereCubeCollision(compOther, compActor, _collisionEvent);
			}  break;
			}
		} break;

		case ColliderType::Sphere:
		{
			switch (compOther->GetCollider()->GetColliderType())
			{
			case ColliderType::Cube:
			{
				SphereCubeCollision(compActor, compOther, _collisionEvent);
				
			} break;

			case ColliderType::Sphere:
			{
				// Sphere to Sphere Collision Detection
				SphereCollision(compActor, compOther, _collisionEvent);
				
			} break;
			}

		} break;
		}

		// Update collision status for rendering
		compActor->m_IsColliding = true;
		compOther->m_IsColliding = true;

		// Call client side implementation
		if(compActor->GetParentTransform()->GetMoveability() == Moveability::Dynamic)
			_collisionEvent.Actor->OnCollision(_collisionEvent.Other, _collisionEvent.ColData);

		if (compOther->GetParentTransform()->GetMoveability() == Moveability::Dynamic)
			_collisionEvent.Other->OnCollision(_collisionEvent.Actor, _collisionEvent.ColData);
	}

	void PhysicsSubsystem::CubeCollision(const Ref<PhysicsComponent>& _cubeCompA, const Ref<PhysicsComponent>& _cubeCompB, CollisionEvent& _collisionEvent)
	{
		// Impulse response
		glm::vec3 compBVelocity = _cubeCompB->GetVelocity();

		glm::vec3 contactForce = -glm::vec3(0.0f, _cubeCompA->m_Mass * -m_ActiveScene->GetGravity(), 0.0f);

		if (_cubeCompA->m_ImpulseEnabled)
		{
			float impulseEnergy = ImpulseEnergy(
				_cubeCompA, _cubeCompB, _collisionEvent.ColData.Normal);

			glm::vec3 impulseVector = impulseEnergy * _collisionEvent.ColData.Normal;

			_cubeCompA->m_Force = glm::vec3(0.0f);
			_cubeCompA->m_Velocity = impulseVector / _cubeCompA->m_Mass;

		}

		_cubeCompA->AddForce(contactForce);
	}

	void PhysicsSubsystem::SphereCollision(const Ref<PhysicsComponent>& _sphereCompA, const Ref<PhysicsComponent>& _sphereCompB, CollisionEvent& _collisionEvent)
	{
		glm::vec3& position = _sphereCompA->m_ParentTransform->GetPosition();
		glm::vec3& positionB = _sphereCompB->m_ParentTransform->GetPosition();
		glm::vec3 positionDistance = position - positionB;
		glm::vec3 normal = glm::normalize(positionDistance);
		float r1 = CastRef<SphereCollider>(_sphereCompA->GetCollider())->GetRadius();
		float r2 = CastRef<SphereCollider>(_sphereCompB->GetCollider())->GetRadius();
		glm::vec3 otherVelocity = _sphereCompB->GetVelocity();
		glm::vec3 velocityA = _sphereCompA->GetVelocity();
		glm::vec3 velocityB = _sphereCompB->GetVelocity();
		glm::vec3 relativeVelocity = velocityA - velocityB;
		glm::vec3 cp = _collisionEvent.ColData.ContactPoint;

		float distance = glm::length(positionDistance);
		
		ImpulseResponse(_sphereCompA, _sphereCompB);

		float penetration = glm::abs(r1 + r2 - distance);
		float mass = _sphereCompA->m_Mass;

		float massOther = _sphereCompB->m_Mass;
		float inverseMass = 1.0f / mass;
		float inverseMassOther = 1.0f / massOther;
		float totalInverseMass = inverseMass + inverseMassOther;
		float totalMass = mass + massOther;

		glm::vec3 mov = position + penetration * (inverseMass / totalInverseMass) * normal;
		position = mov ;

		mov = positionB + penetration * (inverseMassOther / totalInverseMass) * normal;
		positionB = mov;
	}

	void PhysicsSubsystem::SphereCubeCollision(const Ref<PhysicsComponent>& _sphereComp, const Ref<PhysicsComponent>& _cubeComp, CollisionEvent& _collisionEvent)
	{
		// Impulse response
		glm::vec3 otherVelocity = _cubeComp->GetVelocity();

		/*float impulseEnergy = ImpulseEnergy(
			_sphereComp, _cubeComp, _collisionEvent.ColData.Normal, _collisionEvent.ColData.ContactPoint);

		glm::vec3 impulseVector = impulseEnergy * _collisionEvent.ColData.Normal;

		_sphereComp->m_Force = -glm::vec3(0.0f, _sphereComp->m_Mass * -m_ActiveScene->GetGravity(), 0.0f);
		_sphereComp->m_Velocity += impulseVector / _sphereComp->m_Mass;*/

		ImpulseResponse(_sphereComp, _cubeComp);

		const Ref<SphereCollider>& sphereCollider = CastRef<SphereCollider>(_sphereComp->GetCollider());
		const Ref<CubeCollider>& cubeCollider = CastRef<CubeCollider>(_cubeComp->GetCollider());
		const glm::vec3& max = cubeCollider->GetMax();
		glm::vec3& position = _sphereComp->m_ParentTransform->GetPosition();
		position = { position.x, max.y + sphereCollider->GetRadius() * 2.0f, position.z };
	}

	void PhysicsSubsystem::PhysicsSimulationPhase(float _deltaTime)
	{
		for (const auto& physicsComp : m_PhysicsComponents)
		{
			if (!physicsComp->m_SimulatePhysics) continue;

			if (!physicsComp->m_SteppedThisFrame) physicsComp->StepSimulation(_deltaTime);
		}
	}
	void PhysicsSubsystem::ImpulseResponse(const Ref<PhysicsComponent>& _compA, const Ref<PhysicsComponent>& _compB)
	{
		glm::vec3 positionDistance = _compA->m_ParentTransform->GetPosition() - _compB->m_ParentTransform->GetPosition();
		glm::vec3 normal = glm::normalize(positionDistance);
		glm::vec3& velocityA = _compA->m_Velocity;
		glm::vec3& velocityB = _compB->m_Velocity;
		glm::vec3 relativeVelocity = velocityA - velocityB;

		float elasticity = _compA->m_Elasticity + _compB->m_Elasticity;
		float inverseMassA = 1.0f / _compA->GetMass();
		float inverseMassB = 1.0f / _compB->GetMass();
		float jNumerator = -(1.0f + elasticity) * glm::dot(relativeVelocity, normal);
		float totalInverseMass = inverseMassA + inverseMassB;
		float j = jNumerator / totalInverseMass;

		glm::vec3 impulse = j * normal;

		// Add Contact Force
		_compA->m_Force = -glm::vec3(0.0f, _compA->GetMass() * -m_ActiveScene->GetGravity(),0.0f);
		_compB->m_Force = -glm::vec3(0.0f, _compB->GetMass() * -m_ActiveScene->GetGravity(), 0.0f);

		_compA->m_Velocity += impulse * inverseMassA;
		_compB->m_Velocity -= impulse * inverseMassB;
	}
	float PhysicsSubsystem::ImpulseEnergy(const Ref<PhysicsComponent>& _compA, const Ref<PhysicsComponent>& _compB, const glm::vec3& _normal)
	{
		float impulseEnergy = -(1.0f + _compA->m_Elasticity) *
			glm::dot(_compA->m_Velocity - _compB->m_Velocity, _normal) / (1.0f / _compB->m_Mass);

		return impulseEnergy;
	}
}