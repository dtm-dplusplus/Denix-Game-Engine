#pragma once

#include "Denix/System/Subsystem.h"
#include "Denix/Core/Logger.h"
#include "Denix/Physics/PhysicsComponent.h"

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

		void SetActiveScene(const Ref<Scene>& _scene) { m_ActiveScene = _scene; }

		void RegisterComponent(const Ref<PhysicsComponent>& _component);
		void UnregisterComponent(const Ref<PhysicsComponent>& _component);
		void UpdateCollisionDetection(float _deltaTime);
		void Update(float _deltaTime) override;
		void LateUpdate(float _deltaTime) override;

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

		Ref<Scene> m_ActiveScene;

		/** Determines if collisions should be queried when editing the scene */
		bool m_PreviewCollisions = true;
	};
}
