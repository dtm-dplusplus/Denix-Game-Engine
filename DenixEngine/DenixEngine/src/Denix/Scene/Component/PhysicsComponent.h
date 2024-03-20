#pragma once

#include "Denix/Core.h"
#include <glm/vec3.hpp>

#include "ColliderComponent.h"
#include "Denix/Scene/Component.h"

namespace Denix
{
	class PhysicsComponent : public Component, public std::enable_shared_from_this<PhysicsComponent>
	{
	public:
		PhysicsComponent() : Component(ObjectInitializer("Physics Component"))
		{
			m_ColliderComponent = MakeRef<ColliderComponent>();
		}
		PhysicsComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Physics Component")) 
		{
			m_ColliderComponent = MakeRef<ColliderComponent>(m_ID);
		}

		enum class StepMethod
		{
			Euler,
			RK2,
			RK4,
			Verlet
		};

		// Destructors
		~PhysicsComponent() override = default;

	public:
		void Step(float _deltaTime)
		{
			// Step integration
			switch (m_StepMethod)
			{
			case StepMethod::Euler:
				StepEuler(_deltaTime);
				break;

			case StepMethod::RK2:
				StepRK2(_deltaTime);
				break;

			case StepMethod::RK4:
				StepRK4(_deltaTime);
				break;

			case StepMethod::Verlet:
				StepVerlet(_deltaTime);
				break;
			}
		}
		void StepEuler(float _deltaTime)
		{
			// Calculate the net force - Null effect if Drag = 0
			m_Force -= m_Drag * m_Velocity;

			// Calculate new velocity at time t + dt
			m_Velocity += m_Force / m_Mass * _deltaTime;

			// Calculate new displacement at time t + dt
			m_ActorTransformComponent->GetPosition() += m_Velocity * _deltaTime;
		}

		void StepRK2(float _deltaTime)
		{
			glm::vec3 netF;
			glm::vec3 accel;

			// Calculate k1
			netF = m_Force - m_Drag * m_Velocity;
			accel = netF / m_Mass;
			const glm::vec3 k1 = accel * _deltaTime;

			// Calculate k2
			netF = m_Force - m_Drag * (m_Velocity + k1);
			accel = netF / m_Mass;
			const glm::vec3 k2 = accel * _deltaTime;

			// Calculate new velocity at time t + dt
			m_Velocity += (k1 + k2) / 2.f;

			// Calculate new displacement at time t + dt
			m_ActorTransformComponent->GetPosition() += m_Velocity * _deltaTime;
		}

		void StepRK4(float _deltaTime)
		{
		}

		void StepVerlet(float _deltaTime)
		{
		}

		void ToggleSimulation()
		{
			m_IsSimulated = !m_IsSimulated;

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
			m_IsCustomGravity = !m_IsCustomGravity;

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

		Ref<ColliderComponent> GetColliderComponent() { return m_ColliderComponent; }

	public:
		void BeginScene() override;

		void EndScene() override;

		void RegisterComponent() override;

		void UnregisterComponent() override;

	private:
		/** Set to decide if the physics component should update simulation */
		bool m_IsSimulated = true;

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

	private:
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
		Ref<ColliderComponent> m_ColliderComponent;

		/** Transform component which is attached to this components game object */
		Ref<TransformComponent> m_ActorTransformComponent;

		friend class SceneSubsystem;
		friend class PhysicsSubsystem;
		friend class GameObject;
	};
}

