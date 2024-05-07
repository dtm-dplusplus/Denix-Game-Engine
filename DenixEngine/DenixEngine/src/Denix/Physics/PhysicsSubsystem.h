#pragma once

#include "Denix/System/Subsystem.h"
#include "Denix/Core/Logger.h"
#include "Denix/Physics/PhysicsComponent.h"

namespace Denix
{
	class Scene;

	struct CollisionEvent
	{
		Ref<GameObject> Actor;
		Ref<GameObject> Other;
		CollisionData ColData;
	};

	struct TriggerEvent
	{
		Ref<GameObject> Actor;
		Ref<GameObject> Other;
		TriggerData TrigData;
	};

	class PhysicsSubsystem : public Subsystem
	{
	public:
		PhysicsSubsystem()
		{
			s_PhysicsSubSystem = this;

			DE_LOG_CREATE(LogPhysics)
		}

		~PhysicsSubsystem() override
		{
			s_PhysicsSubSystem = nullptr;
		}

		static bool CollisionDetectionEnabled() { return s_PhysicsSubSystem->m_CollisionDetectionEnabled; }
		static bool& CollisionDetectionEnabledRef() { return s_PhysicsSubSystem->m_CollisionDetectionEnabled; }

		static bool CollisionResponseEnabled() { return s_PhysicsSubSystem->m_CollisionResponseEnabled; }
		static bool& CollisionResponseEnabledRef() { return s_PhysicsSubSystem->m_CollisionResponseEnabled; }

	private:
		void CollisionDetection(float _deltaTime);
		void CollisionResonse(float _deltaTime);
		void PhysicsSimulation(float _deltaTime);

		bool BroadCollisionDetection(const Ref<PhysicsComponent>& _component, const Ref<PhysicsComponent>& _otherComponent);
		void NarrowCollisionDetection(const Ref<PhysicsComponent>& _component, const Ref<PhysicsComponent>& _otherComponent);

		void StepPhysicsComponent(const Ref<PhysicsComponent> _component, float _deltaTime);

		/*
		A signed distance 'd' from a point in space 'p'
		to a plane is calculated by a dot product of the plane's normal vector 'n'
		with the vector '(p-q)', where 'q' is any point on the plane
		*/
		float DistanceToPlane(const glm::vec3& n, const glm::vec3& p, const glm::vec3& q);

		/*
		A sphere to a plane collision detection is calculated by finding the parameter between the centre of the sphere c0
		at time step t0 and predict its movement using 'v'. Then find the position of the centre of the sphere using
		the parameter 't'. This also returns new centre point
		*/
		bool MovingSphereToPlaneCollision(const glm::vec3& n, const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& q, float r, glm::vec3& ci);


		/*
		Sphere to sphere collision detection is calculated by finding the distance between the centre of the sphere c0
		and the centre of the sphere c1. This function also finds the contact point cp on the sphere
		*/
		bool SphereToSphereCollision(const glm::vec3& c0, const glm::vec3 c1, float r1, float r2, glm::vec3& cp);

	

	public:
		void SetActiveScene(const Ref<Scene>& _scene) { m_ActiveScene = _scene; }

		void PreUpdate(float _deltaTime) override;

		void Update(float _deltaTime) override;

		void RegisterComponent(const Ref<PhysicsComponent>& _component);

		void UnregisterComponent(const Ref<PhysicsComponent>& _component);

		static PhysicsSubsystem* Get() { return s_PhysicsSubSystem; }

		void Initialize() override
		{
			DE_LOG(LogPhysics, Trace, "PhysicsSubsystem Initialized")

				m_Initialized = true;
		}

		void Deinitialize() override
		{
			DE_LOG(LogPhysics, Trace, "PhysicsSubsystem Deinitialized")
				m_Initialized = false;
		}
	private:
		static PhysicsSubsystem* s_PhysicsSubSystem;

		std::vector<Ref<PhysicsComponent>> m_PhysicsComponents;

		std::vector<CollisionEvent> m_CollisionEvents;
		std::vector <TriggerEvent> m_TriggerEvents;

		Ref<Scene> m_ActiveScene;

		bool m_CollisionDetectionEnabled = true;
		bool m_CollisionResponseEnabled = true;
	};
}