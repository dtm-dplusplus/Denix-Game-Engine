#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Math.h"

#include "Denix/Scene/Component.h"
#include "Denix/Scene/Component/TransformComponent.h"

namespace Denix
{
	class Collider;

	
	struct CollisionData
	{
		glm::vec3 Normal;
		glm::vec3 ContactPoint;
		float Penetration;
	};

	enum class TriggerState
	{
		Null,
		Enter,
		Exit,
		Stay
	};

	
	

	enum class StepMethod
	{
		Euler,
		RK2,
		RK4,
		Verlet
	};

	class PhysicsComponent : public Component, public std::enable_shared_from_this<PhysicsComponent>
	{
	public:
		PhysicsComponent();

		PhysicsComponent(const std::string& _parentName);

		~PhysicsComponent() override = default;

		void StepSimulation(float _deltaTime, bool _nextFrame = false)
		{
			switch (m_StepMethod)
			{
			case StepMethod::Euler:
				ComputeStepEuler(_deltaTime);
				break;

			case StepMethod::RK2:
				ComputeStepRK2(_deltaTime);
				break;

			case StepMethod::RK4:
				ComputeStepRK4(_deltaTime);
				break;

			case StepMethod::Verlet:
				ComputeStepVerlet(_deltaTime);
				break;

			default:; // assert here
			}

			m_SteppedThisFrame = true;
			m_SteppedNextFrame = _nextFrame;
		}


		void AddForce(const glm::vec3& _force)
		{
			m_Force += _force;
		}

		void AddTorque(const glm::vec3& _torque)
		{
			m_Torque += _torque;
		}

		Ref<Collider> GetCollider() const { return m_Collider; }
		Ref<Collider>& GetCollider() { return m_Collider; }
		void SetCollider(const Ref<Collider>& _collider) { m_Collider = _collider; }

	public:
		/**/
		void ComputeTriggerState()
		{
			/*if (collisionDetected)
			{
				trigger.TrigData.NewState = physicsComp->m_IsColliding ? TriggerState::Stay : TriggerState::Enter;
			}
			else
			{
				trigger.TrigData.NewState = physicsComp->m_IsColliding ? TriggerState::Exit : TriggerState::Null;
			}



			if (trigger.TrigData.NewState != TriggerState::Null) m_TriggerEvents.push_back(trigger);*/
		}

		void ComputeCenterOfMass()
		{
			// Compute the center of mass of the object
			// For now, we will assume the center of mass is at the center of the object
			m_CenterOfMass = m_ParentTransform->GetPosition();
		}

		void ComputeInverseInertiaTensor()
		{
			m_ObjectInteriaTensorInverse = m_RotationMatrix * m_BodyInteriaTensorInverse * glm::transpose(m_RotationMatrix);
		}

		void ComputeStepEuler(float _deltaTime);

		void ComputeStepRK2(float _deltaTime);

		void ComputeStepRK4(float _deltaTime)
		{
		}

		void ComputeStepVerlet(float _deltaTime)
		{
		}

		
		void ToggleSimulation()
		{
			if (m_SimulatePhysics)
			{
				DE_LOG(LogPhysics, Trace, "Physics simulation enabled")
			}
			else
			{
				DE_LOG(LogPhysics, Trace, "Physics simulation disabled")
			}
		}

		void ToggleGravity()
		{
			if (m_SimulateGravity)
			{
				DE_LOG(LogPhysics, Trace, "Custom Gravity enabled")
			}
			else
			{
				DE_LOG(LogPhysics, Trace, "Custom Gravity disabled")
			}
		}

		void ToggleTrigger()
		{
			if (m_IsTrigger)
			{
				DE_LOG(LogPhysics, Trace, "Physics component set to trigger collider")
			}
			else
			{
				DE_LOG(LogPhysics, Trace, "Physics component set to collision collider")
			}
		}

		void ToggleCollisionDetection()
		{
			if (m_CollisionDetectionEnabled)
			{
				DE_LOG(LogPhysics, Trace, "Collision detection enabled")
			}
			else
			{
				DE_LOG(LogPhysics, Trace, "Collision detection disabled")
			}
		}

		// Getters
		bool SimulatePhysics() const { return m_SimulatePhysics; }
		bool& SimulatePhysics() { return m_SimulatePhysics; }

		bool CollisionDetectionEnabled() const { return m_CollisionDetectionEnabled; }
		bool& CollisionDetectionEnabled() { return m_CollisionDetectionEnabled; }
		void SetCollisionDetectionEnabled(const bool _collisionDetectionEnabled)
		{
			m_CollisionDetectionEnabled = _collisionDetectionEnabled;

			if (m_CollisionDetectionEnabled)
			{
				DE_LOG(LogPhysics, Trace, "Collision detection enabled")
			}
			else
			{
				DE_LOG(LogPhysics, Trace, "Collision detection disabled")
			}
		}

		void SetStepMethod(const StepMethod _method)
		{
			m_StepMethod = _method;


			DE_LOG(LogPhysics, Trace, "Physics step method set to: {}", GetStepMethod(_method))
		}
		StepMethod GetStepMethod() const { return m_StepMethod; }
		std::string GetStepMethod(StepMethod _method) const
		{
			const static std::string s[]{ "Euler", "k2", "k4", "Verlet" };
			return s[static_cast<int>(m_StepMethod)];
		}

		bool IsTrigger() const { return m_IsTrigger; }
		bool& IsTrigger() { return m_IsTrigger; }
		void SetTrigger(const bool _isTrigger)
		{
			m_IsTrigger = _isTrigger;

			if (m_IsTrigger)
			{
				DE_LOG(LogPhysics, Trace, "Physics component set to trigger collider")
			}
			else
			{
				DE_LOG(LogPhysics, Trace, "Physics component set to collision collider")
			}
		}

		TriggerState GetTriggerState() const { return m_TriggerState; }
		TriggerState& GetTriggerState() { return m_TriggerState; }

		std::string GetTriggerStateS()
		{
			const static std::string s[]{ "Null", "Enter", "Exit", "Stay" };
			return s[static_cast<int>(m_TriggerState)];
		}
		void SetTriggerState(const TriggerState _triggerState)
		{
			m_TriggerState = _triggerState;
		}

		glm::vec3 GetVelocity() const { return m_Velocity; }
		glm::vec3& GetVelocity() { return m_Velocity; }

		float GetMinimumVelocity() const { return m_MinimumVelocity; }	
		float& GetMinimumVelocity() { return m_MinimumVelocity; }

		glm::vec3 GetAngularVelocity() const { return m_AngularVelocity; }
		glm::vec3& GetAngularVelocity() { return m_AngularVelocity; }

		glm::vec3 GetAcceleration() const { return m_Acceleration; }
		glm::vec3& GetAcceleration() { return m_Acceleration; }

		glm::vec3 GetForce() const { return m_Force; }
		glm::vec3& GetForce() { return m_Force; }

		float GetMass() const { return m_Mass; }
		float& GetMass() { return m_Mass; }

		float GetElasticity() const { return m_Elasticity; }
		float& GetElasticity() { return m_Elasticity; }
		void SetElasticity(const float _elasticity)
		{
			m_Elasticity = _elasticity;
		}

		glm::vec3 GetCenterOfMass() const { return m_CenterOfMass; }
		glm::vec3& GetCenterOfMass() { return m_CenterOfMass; }

		glm::vec3 GetMomentOfInertia() const { return m_MomentOfInertia; }
		glm::vec3& GetMomentOfInertia() { return m_MomentOfInertia; }
		
		float GetLinearDrag() const { return m_LinearDrag; }
		float& GetLinearDrag() { return m_LinearDrag; }

		float GetAngularDrag() const { return m_AngularDrag; }
		float& GetAngularDrag() { return m_AngularDrag; }

		bool GetSimulateGravity() const { return m_SimulateGravity; }
		bool& GetSimulateGravity() { return m_SimulateGravity; }
		void SetSimulateGravity(const bool _simulateGravity) { m_SimulateGravity = _simulateGravity; }
		
		bool GetImpulseEnabled() const { return m_ImpulseEnabled; }
		bool& GetImpulseEnabled() { return m_ImpulseEnabled; }
		void SetImpulseEnabled(const bool _impulseEnabled) { m_ImpulseEnabled = _impulseEnabled; }

		Ref<TransformComponent> GetParentTransform() { return m_ParentTransform; }
	public:
		void BeginScene() override;

		void EndScene() override;

		void RegisterComponent() override;

		void UnregisterComponent() override;
		

	private:
		/** Set to decide if the physics component should update simulation */
		bool m_SimulatePhysics = false;

		/** Set to decide if the physics component should perform collision detection */
		bool m_CollisionDetectionEnabled = true;

		/** Set to decide if the physics component should override collision tolereance used by collision system */
		bool m_CustomCollisonTolerance = false;

		/** Custom collision tolerance value which overrides the value used by the collision system */
		float m_CollisionTolerance = 0.5f;

		/** Set to decide if the physics component should be a trigger collider 
		 * If true, the physics component will not respond to collisions, but will still trigger events
		*/
		bool m_IsTrigger = false;

		/** Apply gravitaional force to this object */
		bool m_SimulateGravity = true;

		bool m_ImpulseEnabled = true;

		/** Method used to step the physics simulation */
		StepMethod m_StepMethod = StepMethod::Euler;

		TriggerState m_TriggerState = TriggerState::Null;

		/** Collider used to compute collision responses. Belongs to the physics component */
		Ref<Collider> m_Collider;

		/** Transform component which is attached to this components game object */
		Ref<class TransformComponent> m_ParentTransform;

	private:
		bool m_SteppedThisFrame = false;
		bool m_SteppedNextFrame = false;

		glm::vec3 m_PreviousPosition = glm::vec3(0.f);

		/** Mass of the object */
		float m_Mass = 1.0f;

		/** Center of mass of the object */
		glm::vec3 m_CenterOfMass = glm::vec3(0.f);

		/** Mass Moment of inertia of the object */
		glm::vec3 m_MomentOfInertia = glm::vec3(0.f);

		/** Linear Drag force acting on the object */
		float m_LinearDrag = 0.01f;

		/** Angular Drag force acting on the object */
		float m_AngularDrag = 0.5f;

		/** Elasticity used for impulse response (Bounciness) */
		float m_Elasticity = 0.5f;

		/** Velocity of the object */
		glm::vec3 m_Velocity = glm::vec3(0.f);

		/** Minimum velocity of the object. Used to stop negligble bouncing */
		float m_MinimumVelocity = 0.05f;

		/** Angular velocity of the object */
		glm::vec3 m_AngularVelocity = glm::vec3(0.f);

		/** Angular Momentum */
		glm::vec3 m_AngularMomentum = glm::vec3(0.f);

		/** Acceleration of the object */
		glm::vec3 m_Acceleration = glm::vec3(0.f);

		/** Force acting on the object */
		glm::vec3 m_Force = glm::vec3(0.f);

		float m_Radius = 1.0f; // Radius only applies to sphere colliders

		// Net Torque
		glm::vec3 m_Torque = glm::vec3(0.f);

		// Object Inverse Intertia Tensor
		glm::mat3 m_ObjectInteriaTensorInverse = glm::mat3(1.0f);

		// Body Inertia Tensor Inverse
		glm::mat3 m_BodyInteriaTensorInverse = glm::mat3(1.0f);

		// Rotation Matrix
		glm::mat3 m_RotationMatrix = glm::mat3(1.0f);

		friend class EditorSubsystem;
		friend class SceneSubsystem;
		friend class PhysicsSubsystem;
		friend class CollisionDetection;
		friend class GameObject;
	};
}

