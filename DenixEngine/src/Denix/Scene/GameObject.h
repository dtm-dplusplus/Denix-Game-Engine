#pragma once

#include <ranges>

#include "Denix/Scene/BaseObject.h"
#include "Denix/Scene/Component.h"

#include "Denix/Video/Renderer/RenderComponent.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Physics/PhysicsComponent.h"
#include "Denix/Video/GL/MeshComponent.h"

namespace Denix
{
	/*	Base class for all game objects
	*
	*/
	class GameObject : public BaseObject, public std::enable_shared_from_this<GameObject>
	{
	public:
		// Constructors
		GameObject();
		GameObject(const ObjectInit& _object_init);

		// Destructors
		~GameObject() override = default;

		// Reflection
		void Serialize(YAML::Emitter& _out) override;
		void Deserialize(const YAML::Node& _in) override;

		    

		template<class T, typename ... Args>
		Ref<T> AddComponent(Args&& ... _args)
		{
			if(Ref<T> component = MakeRef<T>(std::forward<Args>(_args)...))
			{
				component->m_Parent = this;
				m_Components[ReflectionHelper::GetDEClassName<T>()] = component;
				return component;
			}

			return nullptr;
		}

		template<typename T>
		Ref<T> GetComponent() const
		{
			for (const auto& [compClass, comp] : m_Components)
			{
				if (const std::string className = ReflectionHelper::GetDEClassName<T>();
					comp->m_ClassName == className)
				{
					return CastRef<T>(comp);
				}
			}

			return nullptr;
		}

		Ref<TransformComponent> GetTransformComponent() { return m_TransformComponent; }

		Ref<PhysicsComponent> GetPhysicsComponent() { return m_PhysicsComponent; }
		Ref<Collider> GetCollider() const { return m_PhysicsComponent->GetCollider(); }

		Ref<MeshComponent> GetMeshComponent() { return m_MeshComponent; }

		Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

		// Physics Component
		virtual void OnCollision(Ref<GameObject>& _other, CollisionData& _collision) {} //const Ref<GameObject>& _other

		virtual void OnTriggerEnter(Ref<GameObject> _other);
		virtual void OnTriggerStay(Ref<GameObject> _other);
		virtual void OnTriggerExit(Ref<GameObject> _other);

		void Destroy()
		{
			// Add more clean up code here
			m_IsRubbish = true;
		}

	public:
		void BeginScene() override
		{
			BaseObject::BeginScene();

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

			BaseObject::EndScene();
		}


		void BeginPlay() override
		{
			BaseObject::BeginPlay();

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

			BaseObject::EndPlay();
		}

		void Update(float _deltaTime) override
		{
			BaseObject::Update(_deltaTime);

			for(const auto& component : m_Components | std::views::values)
			{
			    component->Update(_deltaTime);
            }
		}

	protected:
		std::map<std::string, Ref<Component>> m_Components;

		Ref<TransformComponent> m_TransformComponent;

		Ref<PhysicsComponent> m_PhysicsComponent;

		Ref<MeshComponent> m_MeshComponent;

		Ref<RenderComponent> m_RenderComponent;

		friend class SceneSubsystem;
		friend class Scene;
	};
}
