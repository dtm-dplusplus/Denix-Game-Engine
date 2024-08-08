#pragma once

#include <ranges>

#include "Denix/Scene/Object.h"
#include "Denix/Scene/Component.h"

#include "Denix/Video/Renderer/RenderComponent.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Physics/PhysicsComponent.h"
#include "Denix/Video/GL/MeshComponent.h"

namespace Denix
{
	enum class LayerType
	{
        Default,
		UI
    };

	/*	Base class for all game objects
	*
	*/
	class GameObject : public Object
	{
	public:
		// Constructors
		GameObject(const ObjectInitializer& _object_init = ObjectInitializer::Get());

		// Destructors
		~GameObject() override = default;

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
		Ref<Collider> GetCollider() { return m_PhysicsComponent->GetCollider(); }

		Ref<MeshComponent> GetMeshComponent() { return m_MeshComponent; }

		Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

		// Physics Component
		virtual void OnCollision(Ref<GameObject>& _other, CollisionData& _collision) {} //const Ref<GameObject>& _other

		virtual void OnTriggerEnter(Ref<GameObject> _other) {}
		virtual void OnTriggerStay(Ref<GameObject> _other) {}
		virtual void OnTriggerExit(Ref<GameObject> _other) {}

		void Destroy()
		{
			// Add more clean up code here

			m_IsRubbish = true;
		}

	public:
		void BeginScene() override
		{
			Object::BeginScene();

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

			Object::EndScene();
		}


		void BeginPlay() override
		{
			Object::BeginPlay();

			for (const auto& component : m_Components | std::views::values)
			{
				component->BeginPlay();
			}
		}

		void EndPlay() override
		{
			for (const auto& component : m_Components | std::views::values)
			{
				component->EndPlay();
			}

			Object::EndPlay();
		}

		void Update(float _deltaTime) override
		{
			Object::Update(_deltaTime);

			for(const auto& component : m_Components | std::views::values)
			{
			    component->Update(_deltaTime);
            }
		}
		
		virtual void GameUpdate(float _deltaTime) override
		{
            Object::GameUpdate(_deltaTime);

			for (const auto& component : m_Components | std::views::values)
			{
                component->GameUpdate(_deltaTime);
            }
        }

	protected:
		LayerType m_Layer = LayerType::Default;

		std::unordered_map<std::string, Ref<Component>> m_Components;

		Ref<TransformComponent> m_TransformComponent;

		Ref<PhysicsComponent> m_PhysicsComponent;

		Ref<MeshComponent> m_MeshComponent;

		Ref<RenderComponent> m_RenderComponent;

		friend class SceneSubsystem;
	};
}
