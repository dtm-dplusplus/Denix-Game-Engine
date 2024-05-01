#pragma once

#include "Denix/Core.h"
#include <glm/vec3.hpp>

#include "Denix/Scene/Component.h"
#include "Denix/Scene/GameObjectData.h"

namespace Denix
{
	class Collider;

	enum class TriggerState
	{
		Null,
		Enter,
		Exit,
		Stay
	};

	class PhysicsComponent : public Component, public std::enable_shared_from_this<PhysicsComponent>
	{
	public:
		PhysicsComponent();

		PhysicsComponent(const std::string& _parentName);

		enum class StepMethod
		{
			Euler,
			RK2,
			RK4,
			Verlet
		};

		// Destructors
		~PhysicsComponent() override = default;

		void SetMoveability(const Moveability _moveability)
		{
			m_ParentMoveability = _moveability;
		}

		void AddForce(const glm::vec3& _force)
		{
			m_Force += _force;
		}
	public:

		void ComputeCenterOfMass();
		void Step(float _deltaTime);

		void StepEuler(float _deltaTime);

		void StepRK2(float _deltaTime);

		void StepRK4(float _deltaTime)
		{
		}

		void StepVerlet(float _deltaTime)
		{
		}

		void ToggleSimulation()
		{
			if (m_IsSimulated)
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
			if (m_IsCustomGravity)
			{
				DE_LOG(LogPhysics, Trace, "Custom Gravity enabled")
			}
			else
			{
				DE_LOG(LogPhysics, Trace, "Custom Gravity disabled")
				m_RequestSceneGravity = true;
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
		// Getters
		bool IsSimulated() const { return m_IsSimulated; }
		bool& IsSimulated() { return m_IsSimulated; }

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

		bool IsCustomGravity() const { return m_IsCustomGravity; }
		bool& IsCustomGravity() { return m_IsCustomGravity; }

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

		glm::vec3 GetAngularVelocity() const { return m_AngularVelocity; }
		glm::vec3& GetAngularVelocity() { return m_AngularVelocity; }

		glm::vec3 GetAcceleration() const { return m_Acceleration; }
		glm::vec3& GetAcceleration() { return m_Acceleration; }

		glm::vec3 GetForce() const { return m_Force; }
		glm::vec3& GetForce() { return m_Force; }

		float GetMass() const { return m_Mass; }
		float& GetMass() { return m_Mass; }

		glm::vec3 GetCenterOfMass() const { return m_CenterOfMass; }
		glm::vec3& GetCenterOfMass() { return m_CenterOfMass; }

		glm::vec3 GetMomentOfInertia() const { return m_MomentOfInertia; }
		glm::vec3& GetMomentOfInertia() { return m_MomentOfInertia; }
		glm::vec3 GetDrag() const { return m_Drag; }
		glm::vec3& GetDrag() { return m_Drag; }

		glm::vec3 GetGravity() const { return m_Gravity; }
		glm::vec3& GetGravity() { return m_Gravity; }

		Ref<Collider> GetCollider() { return m_Collider; }

	public:
		void BeginScene() override;

		void EndScene() override;

		void RegisterComponent() override;

		void UnregisterComponent() override;
		

	private:
		/** Set to decide if the physics component should update simulation */
		bool m_IsSimulated = false;

		/** Set to decide if the physics component should be a trigger collider 
		 * If true, the physics component will not respond to collisions, but will still trigger events
		*/
		bool m_IsTrigger = false;
		TriggerState m_TriggerState = TriggerState::Null;

		/** Set to decide if the physics component should use custom gravity
		 * If false, the physics component will use the scene gravity
		 */
		bool m_IsCustomGravity = false;

		/** Set to request the scene gravity from the physics subsystem
		* Used to update the gravity of the physics component if custom gravity is disabled
		*/
		bool m_RequestSceneGravity = false;

		/** Method used to step the physics simulation */
		StepMethod m_StepMethod = StepMethod::Euler;

		bool m_IsColliding = false;
	private:
		/** 
		* 
		* 
		*/
		Moveability m_ParentMoveability = Moveability::Static;
		
		/** Mass of the object */
		float m_Mass = 1.0f;

		/** Center of mass of the object */
		glm::vec3 m_CenterOfMass = glm::vec3(0.f);

		/** Mass Moment of inertia of the object */
		glm::vec3 m_MomentOfInertia = glm::vec3(0.f);

		/** Gravitational force acting on the object */
		glm::vec3 m_Gravity = glm::vec3(0.f, -9.81, 0.f);

		/** Drag force acting on the object */
		glm::vec3 m_Drag = glm::vec3(0.f);

		/** Velocity of the object */
		glm::vec3 m_Velocity = glm::vec3(0.f);

		/** Angular velocity of the object */
		glm::vec3 m_AngularVelocity = glm::vec3(0.f);

		/** Acceleration of the object */
		glm::vec3 m_Acceleration = glm::vec3(0.f);

		/** Force acting on the object */
		glm::vec3 m_Force = glm::vec3(0.f);

		/** Collider used to compute collision responses. Belongs to the physics component */
		Ref<Collider> m_Collider;

		/** Transform component which is attached to this components game object */
		Ref<class TransformComponent> m_ActorTransform;

		friend class SceneSubsystem;
		friend class PhysicsSubsystem;
		friend class GameObject;
	};
}

