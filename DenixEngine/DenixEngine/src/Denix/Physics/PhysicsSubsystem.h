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
		void CollisionDetection(float _deltaTime);
		void CollisionResonse(float _deltaTime);
		void PhysicsSimulation(float _deltaTime);

		
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

		Ref<Scene> m_ActiveScene;

		bool m_CollisionDetectionEnabled = true;
		bool m_CollisionResponseEnabled = true;
	};
}