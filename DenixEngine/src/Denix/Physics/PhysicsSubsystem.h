#pragma once

#include "Denix/System/Subsystem.h"
#include "Denix/Scene/Component/PhysicsComponent.h"

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

		static void RegisterComponent(const Ref<PhysicsComponent>& _comp);
		static void UnregisterComponent(const Ref<PhysicsComponent>& _comp);

		static bool RayCast(const glm::vec3& _origin, const glm::vec3& _direction, float _distance, physx::PxRaycastBuffer& _hit);
		
		inline static physx::PxDefaultAllocator		m_PxAllocator;
		inline static physx::PxDefaultErrorCallback	m_PxErrorCallback;
		inline static physx::PxFoundation*			m_PxFoundation = NULL;
		inline static physx::PxPhysics*				m_PxPhysics	= NULL;
		inline static physx::PxPvd*					m_PxPvd        = NULL;
		inline static physx::PxDefaultCpuDispatcher*	m_PxDispatcher = NULL;
		inline static physx::PxMaterial* m_PxMaterial = nullptr;
		
		static physx::PxScene* CreatePxScene(const physx::PxSceneDesc* _sceneDesc);

	private:
		void Update(float _deltaTime) override;
		
		void Initialize() override;

		void Deinitialize() override;

		static void SetActiveScene(const Ref<Scene>& _scene) { s_Instance->m_ActiveScene = _scene; }

		WRef<Scene> m_ActiveScene;
		
		bool m_CollisionDetectionEnabled = true;
		bool m_CollisionResponseEnabled = true;


		friend class PhysicsComponent;
		friend class SceneSubsystem;
		friend class Engine;
	};
}
