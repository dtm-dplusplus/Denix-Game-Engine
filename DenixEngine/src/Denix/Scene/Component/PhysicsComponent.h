#pragma once

#include <PxRigidBody.h>
#include <foundation/PxTransform.h>

#include "Denix/Core.h"
#include "Denix/Core/Math/Math.h"

#include "Denix/Scene/Component.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Physics/Collider.h"
#include "Denix/Physics/CollisionPair.h"
#include "Denix/Physics/PhysicPrimitive.h"
#include "Denix/Scene/Actor.h"


namespace physx
{
	class PxController;
}

namespace physx
{
	class PxRigidDynamic;
	class PxRigidActor;
}

namespace physx
{
	class PxShape;
}

namespace Denix
{
	class PhysicsComponent : public Component, public std::enable_shared_from_this<PhysicsComponent>
	{
	public:
		PhysicsComponent();

		~PhysicsComponent() override = default;

		void AddForce(const glm::vec3& _force) const;

		void AddAcceleration(const glm::vec3& _acceleration) const;

		void AddImpulse(const glm::vec3& _impulse) const;

		void AddTorque(const glm::vec3& _torque) const;

		bool m_RotationEnabled = true;
		

		void SetInertia();

		float m_PxSlopCoefficient = 0.1f;
		physx::PxRigidBodyFlags m_PxRigidBodyFlags;
		physx::PxShape* m_PxShape = nullptr;
		physx::PxRigidActor* m_PxActor = nullptr;
		physx::PxMaterial* m_PxMaterial = nullptr;
		PhysicsAttributeFlags m_AttributeFlags;

		std::vector<CollisionPair> m_CollisionData;
		
		//physx::PxTransform m_PxTransform = {0.0f, 0.0f, 0.0f};
		ColliderType m_ColliderType = ColliderType::Cube;

		/////////////////////* Linear Properties *///////////////////////
		/** Mass of the object */
		float m_Mass = 1.0f;

		/** Mass Moment of inertia of the object */
		glm::vec3 m_InertiaTensor = glm::vec3(1.0f);

		/** Linear Drag force acting on the object */
		float m_LinearDrag = 0.5f;

		/** Angular Drag force acting on the object */
		float m_AngularDrag = 0.5f;
		
		/** Elasticity used for impulse response (Bounciness) */
		float m_Elasticity = 0.2f;
		float m_StaticFriction = 0.1f;
		float m_DynamicFriction = 0.1f;
		
	private:
		/* Physics Component Settings */
		/** Set to decide if the physics component should update simulation */
		bool m_SimulatePhysics = false;

		/** Set to decide if the physics component should perform collision detection */
		bool m_CollisionDetectionEnabled = true;

		bool m_ImpulseEnabled = true;

		/** Collision used to compute collision responses. Belongs to the physics component */
		Ref<Collider> m_Collider;

	private:
		

		/////////////////////* Angular Properties *///////////////////////
		/** Angular velocity of the object */
		glm::vec3 m_AngularVelocity = glm::vec3(0.f);

		/** Angular Momentum */
		glm::vec3 m_AngularMomentum = glm::vec3(0.f);

		


		/////////////////////* Read Only Properties *///////////////////////
		/** Velocity of the object */
		glm::vec3 m_Velocity = glm::vec3(0.f);

		/** Acceleration of the object */
		glm::vec3 m_Acceleration = glm::vec3(0.f);

		/** Force acting on the object */
		glm::vec3 m_Force = glm::vec3(0.f);
		
		// Net Torque
		glm::vec3 m_Torque = glm::vec3(0.f);
		
	private:
		void RegisterComponent() override;
		void UnregisterComponent() override;

		
		void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
		void PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) override;
		
		void BeginScene() override;
		void EndScene() override;
		
		/* Stateful members below. These dictacte engine behaviour, e.g. IsCollidig determines collider render color */
		bool m_IsColliding = false;

		
		friend class EditorSubsystem;
		friend class SceneSubsystem;
		friend class PhysicsSubsystem;
		friend class CollisionDetection;
		friend class Actor;
		friend class Engine;

	public:
		// Getters
		bool SimulatePhysics() const { return m_SimulatePhysics; }
		bool& SimulatePhysics() { return m_SimulatePhysics; }
		void SetSimulatePhysics(const bool _simulatePhysics)
		{
			m_SimulatePhysics = _simulatePhysics;
		}

		bool CollisionDetectionEnabled() const { return m_CollisionDetectionEnabled; }
		bool& CollisionDetectionEnabled() { return m_CollisionDetectionEnabled; }

		bool IsColliding() const { return m_IsColliding; }

		glm::vec3 GetVelocity() const { return m_Velocity; }
		glm::vec3 GetAngularVelocity() const { return m_AngularVelocity; }
		glm::vec3 GetAcceleration() const { return m_Acceleration; }
		glm::vec3 GetForce() const { return m_Force; }
		float GetMass() const { return m_Mass; }

		bool GetImpulseEnabled() const { return m_ImpulseEnabled; }
		bool& GetImpulseEnabled() { return m_ImpulseEnabled; }
		void SetImpulseEnabled(const bool _impulseEnabled) { m_ImpulseEnabled = _impulseEnabled; }

		Ref<Collider> GetCollider() const;
		Ref<Collider>& GetCollider();

		void SetCollider(const Ref<Collider>& _collider);
	};
}
