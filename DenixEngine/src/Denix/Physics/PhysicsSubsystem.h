#pragma once

#include <concurrent_priority_queue.h>
#include <concurrent_vector.h>

#include "Denix/System/Subsystem.h"
#include "Denix/Core/Logger.h"
#include "Denix/Physics/PhysicsComponent.h"
#include "Denix/Physics/CollisionDetection.h"

#include "PxPhysicsAPI.h"

#define PVD_HOST "127.0.0.1"

namespace Denix
{
	class Scene;

	class PhysicsSubsystem : public Subsystem<PhysicsSubsystem>
	{
	public:
		PhysicsSubsystem()
		{
			DE_LOG_CREATE(LogPhysics)
		}

		~PhysicsSubsystem() override = default;

		PhysicsSubsystem(const PhysicsSubsystem& _other) = delete;
		PhysicsSubsystem(PhysicsSubsystem&& _other) noexcept = delete;
		PhysicsSubsystem& operator=(const PhysicsSubsystem& _other) = delete;
		PhysicsSubsystem& operator=(PhysicsSubsystem&& _other) noexcept = delete;
		
		static bool CollisionDetectionEnabled() { return s_Instance->m_CollisionDetectionEnabled; }
		static bool& CollisionDetectionEnabledRef() { return s_Instance->m_CollisionDetectionEnabled; }

		static bool CollisionResponseEnabled() { return s_Instance->m_CollisionResponseEnabled; }
		static bool& CollisionResponseEnabledRef() { return s_Instance->m_CollisionResponseEnabled; }

		static void RegisterComponent(const Ref<PhysicsComponent>& _component);

		static void UnregisterComponent(const Ref<PhysicsComponent>& _component);

		inline static physx::PxDefaultAllocator		gAllocator;
		inline static physx::PxDefaultErrorCallback	gErrorCallback;
		inline static physx::PxFoundation*			gFoundation = NULL;
		inline static physx::PxPhysics*				gPhysics	= NULL;
		inline static physx::PxPvd*					gPvd        = NULL;
		inline static physx::PxDefaultCpuDispatcher*	gDispatcher = NULL;

		static physx::PxScene* CreatePxScene(const physx::PxSceneDesc* _sceneDesc);

	private:
		void CollisionDetectionPhase(float _deltaTime);
		bool ColllisionExists(const Ref<Actor>& _objectA, const Ref<Actor>& _objectB);
		void CollisionResonsePhase(float _deltaTime);
		void CollisionResponse(CollisionEvent& _collisionEvent);
		void CubeCollision(const Ref<PhysicsComponent>& _cubeCompA, const Ref<PhysicsComponent>& _cubeCompB, CollisionEvent& _collisionEvent);
		void SphereCollision(const Ref<PhysicsComponent>& _sphereCompA, const Ref<PhysicsComponent>& _sphereCompB, CollisionEvent& _collisionEvent);
		void SphereCubeCollision(const Ref<PhysicsComponent>& _sphereComp, const Ref<PhysicsComponent>& _cubeComp, CollisionEvent& _collisionEvent);

		void PhysicsSimulationPhase(float _deltaTime);

		void ImpulseResponse(const Ref<PhysicsComponent>& _compA, const Ref<PhysicsComponent>& _compB); //, const glm::vec3& _normal, const glm::vec3& _contactPoint, float _penetration
		float ImpulseEnergy(const Ref<PhysicsComponent>& _compA, const Ref<PhysicsComponent>& _compB, const glm::vec3& _normal);
	
		void PreUpdate(float _deltaTime) override;

		void Update(float _deltaTime) override;
		
		void Initialize() override;

		void Deinitialize() override;

		std::vector<Ref<PhysicsComponent>>& GetPhysicsComponents() { return m_PhysicsComponents; }
		
		std::vector<CollisionEvent>& GetCollisionEvents() { return m_CollisionEvents; }

		static void SetActiveScene(const Ref<Scene>& _scene) { s_Instance->m_ActiveScene = _scene; }

		std::vector<Ref<PhysicsComponent>> m_PhysicsComponents;
		std::vector<Ref<PhysicsComponent>> m_StaticPhysicsComponents;
		std::vector<Ref<PhysicsComponent>> m_DynamicPhysicsComponents;		
		
		std::vector<CollisionEvent> m_CollisionEvents;

		WRef<Scene> m_ActiveScene;

		bool m_CollisionDetectionEnabled = true;
		bool m_CollisionResponseEnabled = true;

		friend class SceneSubsystem;
		friend class Engine;
	};
}