#pragma once

#include "SubSystem.h"
#include "Denix/Core/Logger.h"
#include "Denix/Scene/Component/PhysicsComponent.h"

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
			DE_LOG(LogPhysics, Trace, "PhysicsComponent Registered: #{} {}", _component->GetID(), _component->GetName())
			m_PhysicsComponents.push_back(_component);
		}

		void UnregisterComponent(const Ref<PhysicsComponent>& _component)
		{
			DE_LOG(LogPhysics, Trace, "PhysicsComponent Unregistered: #{} {}", _component->GetID(), _component->GetName())

			std::erase(m_PhysicsComponents, _component);
		}

		void UnregisterComponent(const int _componentID)
		{
			for (const auto& component : m_PhysicsComponents)
			{
				if (component->GetID() == _componentID)
				{
					DE_LOG(LogPhysics, Trace, "PhysicsComponent Unregistered: #{} {}", component->GetID(), component->GetName())
					std::erase(m_PhysicsComponents, component);
				}
			}
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
