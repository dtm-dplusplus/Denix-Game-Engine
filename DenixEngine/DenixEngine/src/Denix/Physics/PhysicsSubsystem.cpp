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

			// Check if the physicsComp wants the scene gravity value
			if (!_component->m_IsCustomGravity) _component->m_Gravity = m_ActiveScene->GetGravity();

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
			physicsComp->m_Force = glm::vec3(0.0f, physicsComp->m_Mass * -physicsComp->m_Gravity, 0.0f);

			physicsComp->m_PreviousPosition = physicsComp->m_ActorTransform->GetPosition();

			physicsComp->m_CenterOfMass = physicsComp->m_ActorTransform->GetPosition();
		}
	}

	void PhysicsSubsystem::Update(float _deltaTime)
	{
		if (!m_Enabled || !m_ActiveScene->IsPlaying()) return;

		UpdateCollisionDetection(_deltaTime);

		UpdatePhysicsComponents(_deltaTime);
	}
 
	void PhysicsSubsystem::PostUpdate(float _deltaTime)
	{
		Subsystem::PostUpdate(_deltaTime);

		for (const auto& physicsComp : m_PhysicsComponents)
		{
			// Clean collision data
			physicsComp->m_Collisions.clear();
			physicsComp->m_Triggers.clear();

			// Set step status
			physicsComp->m_SteppedThisFrame = physicsComp->m_SteppedNextFrame;
			physicsComp->m_SteppedNextFrame = false;

			// Check if the physicsComp wants the scene gravity value
			if (physicsComp->m_RequestSceneGravity)
			{
				physicsComp->m_Gravity = m_ActiveScene->GetGravity();
				physicsComp->m_RequestSceneGravity = false;
			}
		}
	}

	void PhysicsSubsystem::UpdateCollisionDetection(float _deltaTime)
	{
		for (const auto& physicsComp : m_PhysicsComponents)
		{
			if (physicsComp->m_ActorTransform->GetMoveability() == (int)Moveability::Static && !physicsComp->m_IsTrigger) continue;

			Ref<Collider> collider = physicsComp->m_Collider;
			bool collisionDetected{ false };

			
			glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f);
			glm::vec3 planePos = glm::vec3(0.f);

			// Checks against ground plane for now
			if (float distance = DistanceToPlane(normal, physicsComp->m_ActorTransform->GetPosition(), planePos); distance <= physicsComp->m_Radius)
			{
				collisionDetected = true;

				CollisionDetection collision;
				collision.Normal = normal;

				// If Collider is trigger compute the trigger state
				if (physicsComp->IsTrigger())
				{
					TriggerDetection trigger;

					// Check Trigger State
					if (collisionDetected)
					{
						trigger.State = physicsComp->m_IsColliding ? TriggerState::Stay : TriggerState::Enter;
					}
					else
					{
						trigger.State = physicsComp->m_IsColliding ? TriggerState::Exit : TriggerState::Null;
					}

					physicsComp->m_TriggerState = trigger.State;

					if (trigger.State != TriggerState::Null)
					{
						physicsComp->m_Triggers.push_back(trigger);
					}
				}
				physicsComp->m_Collisions.push_back(collision);
			}

			collider->m_RenderComponent->GetMaterial()->SetBaseColor(
				physicsComp->m_IsColliding ? collider->m_CollisionColor : collider->m_NoCollisionColor);

			physicsComp->m_IsColliding = collisionDetected;

		}
	}

	void PhysicsSubsystem::UpdatePhysicsComponents(float _deltaTime)
	{
		for (const auto& physicsComp : m_PhysicsComponents)
		{
			// Compute Collision Detection
			for (const CollisionDetection& collision : physicsComp->m_Collisions)
			{
				// Impulse response
				glm::vec3 planeVelocity = glm::vec3(0.0f);

				float impulseMag = -(1.0f + physicsComp->m_Elasticity) * 
					glm::dot(physicsComp->m_Velocity - planeVelocity, collision.Normal) / (1.0f / physicsComp->m_Mass);

				glm::vec3 impulseVector = impulseMag * collision.Normal;

				physicsComp->m_Velocity += impulseVector / physicsComp->m_Mass;

				////Contact normal force
				glm::vec3 contactForce = glm::vec3(0.f, -physicsComp->m_Gravity * physicsComp->m_Mass, 0.f);
				physicsComp->AddForce(contactForce);

				// Call client side function
				if (const Ref<GameObject> parentObj = m_ActiveScene->GetGameObject(physicsComp->m_ParentObjectName))
				{
					parentObj->OnCollision(nullptr, collision);
				}
			}

			// Compute Trigger Detection
			for (const TriggerDetection& trigger: physicsComp->m_Triggers)
			{
				// Call client side function
				if (const Ref<GameObject> parentObj = m_ActiveScene->GetGameObject(physicsComp->m_ParentObjectName))
				{
					switch (physicsComp->m_TriggerState)
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
			
			// Skip if not simulated
			if (!physicsComp->m_IsSimulated) continue;

			// Compute Torque
			physicsComp->m_Torque = physicsComp->m_Force * physicsComp->m_Radius;

			// Step Integration
			if (!physicsComp->m_SteppedThisFrame)
			{
				StepPhysicsComponent(physicsComp, _deltaTime);
			}

			// update the angular momentum : L(t + 1) = L(t) + T * dt;
			physicsComp->m_AngularMomentum += physicsComp->m_Radius * physicsComp->m_Mass  * physicsComp->m_Velocity * _deltaTime;

			// compute the inverse inertia tensor I1 = R Ibody 1RT; 
			physicsComp->ComputeInverseInertiaTensor();

			// update the angular velocity wt + 1 = | -1 Lt + 1
			// 	reconstruct the skew matrix w(*) (refer to my lecture note)
			// 	update the rotation matrix R of the rigid body : R(t + 1) = R(t) + dt w(*)R(t);
			// 12) apply the rotation to the rigid body using glm matrix for visualisation;

			
		}
	}

	void PhysicsSubsystem::StepPhysicsComponent(const Ref<PhysicsComponent> _component, float _deltaTime)
	{
		switch (_component->m_StepMethod)
		{
		case StepMethod::Euler:
			_component->StepEuler(_deltaTime);
			break;

		case StepMethod::RK2:
			_component->StepRK2(_deltaTime);
			break;

		case StepMethod::RK4:
			_component->StepRK4(_deltaTime);
			break;

		case StepMethod::Verlet:
			_component->StepVerlet(_deltaTime);
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