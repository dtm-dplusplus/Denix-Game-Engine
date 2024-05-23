#pragma once

#include "Denix/System/Subsystem.h"
#include "Denix/Core/Logger.h"
#include "Denix/Physics/PhysicsComponent.h"
#include "Denix/Physics/CollisionDetection.h"

namespace Denix
{
	class Scene;

	

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
		void CollisionDetectionPhase(float _deltaTime);
		bool ColllisionExists(const Ref<GameObject>& _objectA, const Ref<GameObject>& _objectB);
		void CollisionResonsePhase(float _deltaTime);
		void CollisionResponse(CollisionEvent& _collisionEvent);
		void CubeCollision(const Ref<PhysicsComponent>& _cubeCompA, const Ref<PhysicsComponent>& _cubeCompB, CollisionEvent& _collisionEvent);
		void SphereCollision(const Ref<PhysicsComponent>& _sphereCompA, const Ref<PhysicsComponent>& _sphereCompB, CollisionEvent& _collisionEvent);
		void SphereCubeCollision(const Ref<PhysicsComponent>& _sphereComp, const Ref<PhysicsComponent>& _cubeComp, CollisionEvent& _collisionEvent);

		void PhysicsSimulationPhase(float _deltaTime);

		void ImpulseResponse(const Ref<PhysicsComponent>& _compA, const Ref<PhysicsComponent>& _compB); //, const glm::vec3& _normal, const glm::vec3& _contactPoint, float _penetration
		float ImpulseEnergy(const Ref<PhysicsComponent>& _compA, const Ref<PhysicsComponent>& _compB, const glm::vec3& _normal);
	
	public:

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

		static void SetActiveScene(const Ref<Scene>& _scene) { s_PhysicsSubSystem->m_ActiveScene = _scene; }

		std::vector<Ref<PhysicsComponent>> m_PhysicsComponents;

		std::vector<CollisionEvent> m_CollisionEvents;

		Ref<Scene> m_ActiveScene;

		bool m_CollisionDetectionEnabled = true;
		bool m_CollisionResponseEnabled = true;

		friend class SceneSubsystem;
	};
}