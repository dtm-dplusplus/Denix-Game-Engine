#pragma once

#include "SubSystem.h"
#include "Core/Logger.h"
#include "Scene/Component.h"

namespace Denix
{
	class PhysicsSubSystem : public SubSystem
	{
	public:
		PhysicsSubSystem()
		{
			s_PhysicsSubSystem = this;

			DE_LOG_CREATE(LogPhysics)
		}

		~PhysicsSubSystem() override
		{
			s_PhysicsSubSystem = nullptr;
		}

		void RegisterComponent(const Ref<PhysicsComponent>& _component)
		{
			m_PhysicsComponents.push_back(_component);
		}

		void UnregisterComponent(const Ref<PhysicsComponent>& _component)
		{
			std::erase(m_PhysicsComponents, _component);
		}

		void Update(float _deltaTime) override;
	public: 
		static PhysicsSubSystem* Get() { return s_PhysicsSubSystem; }

		void Initialize() override
		{
			DE_LOG(LogPhysics, Trace, "PhysicsSubSystem Initialized")

			m_Initialized = true;
		}

		void Deinitialize() override
		{
			DE_LOG(LogPhysics, Trace, "PhysicsSubSystem Deinitialized")
			m_Initialized = false;
		}

	private:
		static PhysicsSubSystem* s_PhysicsSubSystem;

		std::vector<Ref<PhysicsComponent>> m_PhysicsComponents;
	};
}
