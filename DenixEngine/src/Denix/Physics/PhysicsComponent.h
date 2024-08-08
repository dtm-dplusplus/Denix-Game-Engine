#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Math.h"

#include "Denix/Scene/Component.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Physics/Collider.h"

namespace Denix
{
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

		void Update(float _deltaTime) override
		{
			Component::Update(_deltaTime);

			m_ParentTransform->m_PhysicsRotationOverride = m_SimulatePhysics;

			if (m_Collider)
			{
				m_Collider->m_TransformComponent->SetPosition(m_ParentTransform->GetPosition());

				switch (m_Collider->GetColliderType())
				{
				case ColliderType::Cube:
				{
					if (!m_CollisonDimesionOverride)
					{
						CastRef<CubeCollider>(m_Collider)->GetDimensions() = m_ParentTransform->GetScale();
					}
				} break;

				case ColliderType::Sphere:
				{
					Ref<SphereCollider> sphereCol = CastRef<SphereCollider>(m_Collider);
					m_MomentOfInertia = (2.0 / 5.0) * m_Mass * pow(sphereCol->GetRadius(), 2);
					m_ParentTransform->SetScale(glm::vec3(sphereCol->GetRadius() * 2.0f));
					m_Collider->GetTransformComponent()->SetScale(m_ParentTransform->GetScale());
				} break;
				}

				m_Collider->Update(_deltaTime);
			}
		}

		void StepSimulation(float _deltaTime);


		void AddForce(const glm::vec3& _force)
		{
			m_Force += _force;
		}

		void AddImpulse(const glm::vec3& _impulse)
		{
            m_Velocity += _impulse / m_Mass;
        }

		void AddTorque(const glm::vec3& _torque)
		{
			m_Torque += _torque;
		}

		

	private:
		/* Physics Component Settings */
		/** Set to decide if the physics component should update simulation */
		bool m_SimulatePhysics = false;

		/** Apply gravitaional force to this object */
		bool m_SimulateGravity = true;

		/** Set to decide if the attached collider should be rendered in default/unlit viewport - Null effect in collision viewport */
		bool m_IsColliderVisible = false;

		/** Set to decide if the physics component should perform collision detection */
		bool m_CollisionDetectionEnabled = true;

		bool m_CollisonDimesionOverride = false;

		/** Set to decide if the physics component should override collision tolereance used by collision system */
		bool m_CustomCollisonTolerance = false;

		/** Custom collision tolerance value which overrides the value used by the collision system */
		float m_CollisionTolerance = 0.5f;

		/** Set to decide if the physics component should be a trigger collider 
		 * If true, the physics component will not respond to collisions, but will still trigger events
		*/
		bool m_IsTrigger = false;

		bool m_ImpulseEnabled = true;

		/** Method used to step the physics simulation */
		StepMethod m_StepMethod = StepMethod::RK2;

		TriggerState m_TriggerState = TriggerState::Null;

		/** Collision used to compute collision responses. Belongs to the physics component */
		Ref<Collider> m_Collider;

		/** Broad collider used for broad phase collision detection */
		Ref<SphereCollider> m_BroadCollider;

		/** Transform component which is attached to this components game object */
		Ref<class TransformComponent> m_ParentTransform;

	private:
		/////////////////////* Linear Properties *///////////////////////
		/** Mass of the object */
		float m_Mass = 1.0f;

		/** Velocity of the object */
		glm::vec3 m_Velocity = glm::vec3(0.f);

		/** Acceleration of the object */
		glm::vec3 m_Acceleration = glm::vec3(0.f);

		/** Force acting on the object */
		glm::vec3 m_Force = glm::vec3(0.f);

		/** Center of mass of the object */
		glm::vec3 m_CenterOfMass = glm::vec3(0.f);

		/** Mass Moment of inertia of the object */
		/*glm::vec3 m_MomentOfInertia = glm::vec3(0.f);*/

		/** Linear Drag force acting on the object */
		float m_LinearDrag = 0.5f;

		/** Elasticity used for impulse response (Bounciness) */
		float m_Elasticity = 0.0f;

		/////////////////////* Angular Properties *///////////////////////
		/** Angular velocity of the object */
		glm::vec3 m_AngularVelocity = glm::vec3(0.f);

		/** Angular Momentum */
		glm::vec3 m_AngularMomentum = glm::vec3(0.f);

		// Net Torque
		glm::vec3 m_Torque = glm::vec3(0.f);

		/** Angular Drag force acting on the object */
		float m_AngularDrag = 0.5f;

		/** Moment of inertia of the object */
		float m_MomentOfInertia = 0.0f;

		// Object Inertia Tensor
		glm::mat3 m_ObjectInteriaTensor = glm::mat3(1.0f);

		// Object Inverse Intertia Tensor
		glm::mat3 m_ObjectInteriaTensorInverse = glm::mat3(1.0f);

		// Body Inertia Tensor
		glm::mat3 m_BodyInteriaTensor = glm::mat3(1.0f);

		// Body Inertia Tensor Inverse
		glm::mat3 m_BodyInteriaTensorInverse = glm::mat3(1.0f);

		private:
			void BeginPlay() override;

			void EndScene() override;

			void RegisterComponent() override;

			void UnregisterComponent() override;

	private:
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

		void ComputeCenterOfMass();

		void ComputeObjectInertiaTensor()
		{
			m_ObjectInteriaTensor = m_ParentTransform->m_RotationMatrix * m_BodyInteriaTensor * glm::transpose(m_ParentTransform->m_RotationMatrix);
		}
		void ComputeObjectInverseInertiaTensor()
		{
			m_ObjectInteriaTensorInverse = m_ParentTransform->m_RotationMatrix *
				m_BodyInteriaTensorInverse * glm::transpose(m_ParentTransform->m_RotationMatrix);
		}

		void ComputeBodyInertiaTensor()
		{
			// Set Inertia Tensor && inverse Inertia Tensor
			if (m_Collider)
			{
				switch (m_Collider->GetColliderType())
				{
				case ColliderType::Cube:
				{
					Ref<CubeCollider> cubeCol = CastRef<CubeCollider>(m_Collider);

				} break;

				case ColliderType::Sphere:
				{
					Ref<SphereCollider> sphereCol = CastRef<SphereCollider>(m_Collider);
					m_BodyInteriaTensor = glm::mat3((2.0f / 5.0f) * m_Mass * pow(sphereCol->GetRadius(), 2));
					m_BodyInteriaTensorInverse = glm::inverse(m_BodyInteriaTensor);
				} break;
				}
			}
		}

		void ComputeTorqueArm(const glm::vec3& _contactPoint, const glm::vec3& _force)
		{
			glm::vec3 torqueArm = _contactPoint - m_CenterOfMass;
			m_Torque += glm::cross(torqueArm, _force);
		}
		void ComputeAngularVelocity()
		{
			m_AngularVelocity = m_ObjectInteriaTensorInverse * m_AngularMomentum;
		}

		void ComputeRotationMatrix(float _deltaTime);

		void ComputeTorque(const glm::vec3& _torqueArm, const glm::vec3& _force)
		{
			m_Torque += glm::cross(_torqueArm, _force);
		}

		glm::mat3 GetSkewMatrix(const glm::vec3& _vector)
		{
			return glm::mat3(
				0.0f, -_vector.z, _vector.y,
				_vector.z, 0.0f, -_vector.x,
				-_vector.y, _vector.x, 0.0f);
		}

		glm::vec3 GetEulerAngles(const glm::mat3& _rotationMatrix) const
		{
			float value = pow(_rotationMatrix[0][0], 2) + pow(_rotationMatrix[1][0], 2);
			float sy = sqrt(value);

			float x, y, z;

			if (sy < 1e-6)
			{
				x = atan2(_rotationMatrix[2][1], _rotationMatrix[2][2]);
				y = atan2(-_rotationMatrix[2][0], sy);
				z = atan2(_rotationMatrix[1][0], _rotationMatrix[0][0]);
			}
			else
			{
				x = atan2(-_rotationMatrix[1][2], _rotationMatrix[1][1]);
				y = atan2(-_rotationMatrix[2][0], sy);
				z = 0.0f;
			}

			return glm::vec3(x, y, z);
		}

		glm::vec3 FrictionForce(const glm::vec3& _relVel, const glm::vec3& _contactNormal,
			const glm::vec3& _forceNormal, float _mu)
		{

		}
		void ComputeStepEuler(float _deltaTime);

		void ComputeStepRK2(float _deltaTime);
	private:
		/* Stateful members below. These dictacte engine behaviour, e.g. IsCollidig determines collider render color */
		bool m_SteppedThisFrame = false;
		bool m_SteppedNextFrame = false;
		bool m_IsColliding = false;
		glm::vec3 m_PreviousPosition = glm::vec3(0.f);

	
		friend class EditorSubsystem;
		friend class SceneSubsystem;
		friend class PhysicsSubsystem;
		friend class CollisionDetection;
		friend class GameObject;

	public:
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
		void SetSimulatePhysics(const bool _simulatePhysics)
		{
			m_SimulatePhysics = _simulatePhysics;

			if (m_SimulatePhysics)
			{
				DE_LOG(LogPhysics, Trace, "Physics simulation enabled")
			}
			else
			{
				DE_LOG(LogPhysics, Trace, "Physics simulation disabled")
			}
		}

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

		bool IsColliding() const { return m_IsColliding; }

		bool IsColliderVisible() const { return m_IsColliderVisible; }
		bool& IsColliderVisible() { return m_IsColliderVisible; }

		bool CollisionDimensionOverride() const { return m_CollisonDimesionOverride; }
		bool& CollisionDimensionOverride() { return m_CollisonDimesionOverride; }

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

		/*float GetMinimumVelocity() const { return m_MinimumVelocity; }
		float& GetMinimumVelocity() { return m_MinimumVelocity; }*/

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

		/*glm::vec3 GetMomentOfInertia() const { return m_MomentOfInertia; }
		glm::vec3& GetMomentOfInertia() { return m_MomentOfInertia; }*/

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

		Ref<Collider> GetCollider() const { return m_Collider; }
		Ref<Collider>& GetCollider() { return m_Collider; }
		void SetCollider(const Ref<Collider>& _collider)
		{
			m_Collider = _collider;

		}
	};
}