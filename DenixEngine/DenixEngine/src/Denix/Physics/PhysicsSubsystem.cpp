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
		m_PhysicsComponents.push_back(_component);
	}

	void PhysicsSubsystem::UnregisterComponent(const Ref<PhysicsComponent>& _component)
	{
		DE_LOG(LogPhysics, Trace, "PhysicsComponent Unregistered: #{} {}", _component->GetID(), _component->GetName())
		std::erase(m_PhysicsComponents, _component);
	}

	void PhysicsSubsystem::PreUpdate(float _deltaTime)
	{
		Subsystem::PreUpdate(_deltaTime);

		for (const auto& physicsComp : m_PhysicsComponents)
		{
			// Clean collision colData
			m_CollisionEvents.clear();
			m_TriggerEvents.clear();

			// Set step status
			physicsComp->m_SteppedThisFrame = physicsComp->m_SteppedNextFrame;
			physicsComp->m_SteppedNextFrame = false;

			physicsComp->m_Force = glm::vec3(0.0f, physicsComp->m_Mass * -m_ActiveScene->GetGravity(), 0.0f);

			//thisComp->m_Torque = glm::vec3(0.0f);

			physicsComp->m_PreviousPosition = physicsComp->m_ActorTransform->GetPosition();

			physicsComp->m_CenterOfMass = physicsComp->m_ActorTransform->GetPosition();
		}
	}

	void PhysicsSubsystem::Update(float _deltaTime)
	{
		if (!m_Enabled || !m_ActiveScene->IsPlaying()) return;

		if(m_CollisionDetectionEnabled) CollisionDetection(_deltaTime);

		PhysicsSimulation(_deltaTime);
	}
 
	void PhysicsSubsystem::CollisionDetection(float _deltaTime)
	{
		// Create a list of static objects and a list of dynamic objects
		std::vector<Ref<PhysicsComponent>> staticObjects;
		std::vector<Ref<PhysicsComponent>> dynamicObjects;

		for (const auto& physicsComp : m_PhysicsComponents)
		{
			if (physicsComp->m_ActorTransform->GetMoveability() == Moveability::Static)
			{
				staticObjects.push_back(physicsComp);
			}
			else
			{
				dynamicObjects.push_back(physicsComp);
			}
		}

		// Check collision detection for dynamic objects
		for (const auto& physicsComp : dynamicObjects)
		{
			// Check collision detection enabled for this component
			if (!physicsComp->CollisionDetectionEnabled()) continue;

			for (const auto& staticComp : staticObjects)
			{
				// Broad Phase Collision Detection
				BroadCollisionDetection(physicsComp, staticComp);

				// Narrow Phase Collision Detection
				NarrowCollisionDetection(physicsComp, staticComp);
				NarrowTriggerDetection(physicsComp, staticComp);

				if (!physicsComp->IsTrigger())
				{
					
				}
				else
				{
					if (!staticComp->IsTrigger())
					{
						// Trigger - Collision Detection
					}
					else
					{
						// Trigger - Trigger Detection
					}
					// Trigger - Collision Detection

				}
			}
			// Broad Phase Collision Detection

			// Create a bounding sphere for the physics component

			// check against all other physics components

			// If collision detected, perform Narrow Phase Collision Detection
			if (!physicsComp->IsTrigger())
			{
				// Narrow Collision Detection



			}
			else
			{
				// Narrow Trigger Detection
			}
			
			
			
			Ref<Collider> collider = physicsComp->m_Collider;
			bool collisionDetected{ false };

			
			glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f);
			glm::vec3 planePos = glm::vec3(0.f);

			switch (collider->GetColliderType())
			{
			case ColliderType::Cube:
			{
				if (float distance = DistanceToPlane(normal, physicsComp->m_ActorTransform->GetPosition(), planePos); distance <= physicsComp->m_Radius)
				{
					collisionDetected = true;

					CollisionEvent collision;
					collision.ColData.Normal = normal;

					if (const Ref<GameObject> thisParent = m_ActiveScene->GetGameObject(physicsComp->m_ParentObjectName))
					{

					}
					// If Collider is trigger compute the trigger state
					
					physicsComp->m_Collisions.push_back(collision);
				}
			} break;

			case ColliderType::Sphere:
			{
				// Check against sphere collider
				if (MovingSphereToPlaneCollision(normal, physicsComp->m_PreviousPosition, physicsComp->m_ActorTransform->GetPosition(), planePos, physicsComp->m_Radius, physicsComp->m_ActorTransform->GetPosition()))
				{
					collisionDetected = true;
				}
			} break;
			}
			
			if (physicsComp->IsTrigger())
			{
				TriggerData trigger;

				// Check Trigger NewState
				if (collisionDetected)
				{
					trigger.NewState = physicsComp->m_IsColliding ? TriggerState::Stay : TriggerState::Enter;
				}
				else
				{
					trigger.NewState = physicsComp->m_IsColliding ? TriggerState::Exit : TriggerState::Null;
				}

				physicsComp->m_TriggerState = trigger.NewState;

				if (trigger.NewState != TriggerState::Null)
				{
					physicsComp->m_Triggers.push_back(trigger);
				}
			}

			collider->m_RenderComponent->GetMaterial()->SetBaseColor(
				physicsComp->m_IsColliding ? collider->m_CollisionColor : collider->m_NoCollisionColor);

			physicsComp->m_IsColliding = collisionDetected;

		}
	}

	void PhysicsSubsystem::PhysicsSimulation(float _deltaTime)
	{
		for (const auto& physicsComp : m_PhysicsComponents)
		{
			if(m_CollisionResponseEnabled) CollisionResonse(physicsComp);
			
			if (!physicsComp->m_SimulatePhysics) continue;

			// Compute Torque
			//thisComp->m_Torque = thisComp->m_Force * thisComp->m_Radius;

			// Step Integration - Collision detection may have already stepped for dynamic objects
			if (!physicsComp->m_SteppedThisFrame) StepPhysicsComponent(physicsComp, _deltaTime);


			// update the angular momentum : L(t + 1) = L(t) + T * dt;
			// thisComp->m_AngularMomentum += thisComp->m_Radius * thisComp->m_Mass  * thisComp->m_Velocity * _deltaTime;

			// compute the inverse inertia tensor I1 = R Ibody 1RT; 
			// thisComp->ComputeInverseInertiaTensor();

			// update the angular velocity wt + 1 = | -1 Lt + 1
			// 	reconstruct the skew matrix w(*) (refer to my lecture note)
			// 	update the rotation matrix R of the rigid body : R(t + 1) = R(t) + dt w(*)R(t);
			// 12) apply the rotation to the rigid body using glm matrix for visualisation;		
		}
	}

	void PhysicsSubsystem::BroadCollisionDetection(const Ref<PhysicsComponent>& _component, const Ref<PhysicsComponent>& _otherComponent)
	{
		// Create a bounding sphere for the physics component

		return false;
	}

	void PhysicsSubsystem::NarrowCollisionDetection(const Ref<PhysicsComponent>& _component, const Ref<PhysicsComponent>& _otherComponent)
	{
	}

	void PhysicsSubsystem::CollisionResonse(float _deltaTime)
	{
		// Compute Collision Detection
		for (const auto& [thisComp, otherComp, colData] : m_CollisionEvents)
		{

			// Impulse response
			glm::vec3 planeVelocity = glm::vec3(0.0f);

			glm::vec3 contactForce = glm::vec3(0.f, thisComp->m_Mass * m_ActiveScene->GetGravity(), 0.f);

			if (thisComp->m_ImpulseEnabled)
			{
				float impulseEnergy = -(1.0f + thisComp->m_Elasticity) *
					glm::dot(thisComp->m_Velocity - planeVelocity, colData.Normal) / (1.0f / thisComp->m_Mass);

				glm::vec3 impulseVector = impulseEnergy * colData.Normal;

				thisComp->m_Velocity += impulseVector / thisComp->m_Mass;
			}
			

			thisComp->AddForce(contactForce);

			// Call client side implementation
			if (const Ref<GameObject> thisParent = m_ActiveScene->GetGameObject(thisComp->m_ParentObjectName))
			{
				thisParent->OnCollision(otherComp, colData);
			}

			if (const Ref<GameObject> otherParent = m_ActiveScene->GetGameObject(otherComp->m_ParentObjectName))
			{
				otherParent->OnCollision(thisComp, colData);
			}
		}

		// Compute Trigger Detection
		for (const auto& [thisComp, otherComp, trigData] : m_CollisionEvents)
		{
			// Call client side function
			if (const Ref<GameObject> parentObj = m_ActiveScene->GetGameObject(thisComp->m_ParentObjectName))
			{
				switch (thisComp->m_TriggerState)
				{
				case TriggerState::Stay:
				{
					parentObj->OnTriggerStay(nullptr);
				} break;

				case TriggerState::Enter:
				{
					parentObj->OnTriggerEnter(nullptr);
				} break;

				case TriggerState::Exit:
				{
					parentObj->OnTriggerExit(nullptr);
				} break;
				}
			}
		}
	}

	void PhysicsSubsystem::StepPhysicsComponent(const Ref<PhysicsComponent> _component, float _deltaTime)
	{
		switch (_component->m_StepMethod)
		{
		case StepMethod::Euler:
			_component->ComputeStepEuler(_deltaTime);
			break;

		case StepMethod::RK2:
			_component->ComputeStepRK2(_deltaTime);
			break;

		case StepMethod::RK4:
			_component->ComputeStepRK4(_deltaTime);
			break;

		case StepMethod::Verlet:
			_component->ComputeStepVerlet(_deltaTime);
			break;

		default:; // assert here
		}

		_component->m_SteppedThisFrame = true;
	}

	float PhysicsSubsystem::DistanceToPlane(const glm::vec3& n, const glm::vec3& p, const glm::vec3& q)
	{
		float d = glm::dot((p - q), n);
		return d;
	}

	bool PhysicsSubsystem::MovingSphereToPlaneCollision(const glm::vec3& n, const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& q, float r, glm::vec3& ci)
	{
		float t;

		float d0 = DistanceToPlane(n, c0, q);
		float d1 = DistanceToPlane(n, c1, q);

		if (glm::abs(d0) <= r)
		{
			ci = c0;
			t = 0.0f;
			return true;
		}
		if (d0 > r && d1 < r)
		{
			t = (d0 - r) / (d0 - d1);
			ci = (1 - t) * c0 + t * c1;
			return true;
		}
		return false;
	}


	bool PhysicsSubsystem::SphereToSphereCollision(const glm::vec3& c0, const glm::vec3 c1, float r1, float r2, glm::vec3& cp)
	{
		float d = glm::length(c0 - c1);
		glm::vec3 n;

		if (d <= (r1 + r2))
		{
			n = glm::normalize(c0 - c1);
			cp = r1 * n;
			return true;
		}
		return false;
	}
}