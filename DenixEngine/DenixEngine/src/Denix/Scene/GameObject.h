#pragma once

#include <ranges>

#include "Object.h"
#include "Component.h"

namespace Denix
{
	/*	Base class for all game objects
	*
	*/
	class GameObject : public Object
	{
	public:
		// Constructors
		GameObject(const ObjectInitializer& _object_init = ObjectInitializer::Get()) : Object(_object_init)
		{
			m_TransformComponent = MakeRef<TransformComponent>(m_ID);
			m_Components["Transform"] = m_TransformComponent;

			m_PhysicsComponent = MakeRef<PhysicsComponent>(m_ID);
			m_Components["Physics"] = m_PhysicsComponent;

			m_MeshComponent = MakeRef<MeshComponent>(m_ID);
			m_Components["Mesh"] = m_MeshComponent;

			m_RenderComponent = MakeRef<RenderComponent>(m_ID);
			m_Components["Render"] = m_RenderComponent;
		}

		// Destructors
		~GameObject() override = default;

		void BeginScene() override
		{
			for (const auto& component : m_Components | std::views::values)
			{
				component->BeginScene();
			}
		}
		void EndScene() override
		{
			for (const auto& component : m_Components | std::views::values)
			{
				component->EndScene();
			}
		}

		void Update(float _deltaTime) override {}

	public:
		Ref<Component> AddComponent(const Ref<Component>& _component)
		{
			m_Components[_component->GetName()] = _component;

			return _component;
		}

		Ref<Component> GetComponent(const std::string& _name)
		{
			if (m_Components.contains(_name))
			{
				return m_Components[_name];
			}

			return nullptr;
		}

		Ref<TransformComponent> GetTransformComponent() { return m_TransformComponent; }

		Ref<PhysicsComponent> GetPhysicsComponent() { return m_PhysicsComponent; }

		Ref<MeshComponent> GetMeshComponent() { return m_MeshComponent; }

		Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

	protected:
		std::unordered_map<std::string, Ref<Component>> m_Components;

		Ref<TransformComponent> m_TransformComponent;

		Ref<PhysicsComponent> m_PhysicsComponent;

		Ref<MeshComponent> m_MeshComponent;

		Ref<RenderComponent> m_RenderComponent;

		friend class SceneSubSystem;
	};
}
