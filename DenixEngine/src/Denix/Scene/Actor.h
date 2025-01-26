#pragma once

#include <ranges>

#include "Denix/Core/BaseObject.h"
#include "Denix/Scene/Component.h"

#include "Denix/Scene/Component/RenderComponent.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Scene/Component/PhysicsComponent.h"
#include "Denix/Scene/Component/ModelComponent.h"

namespace Denix
{
	/*	Base class for all actors
	*
	*/
	class Actor : public BaseObject, public std::enable_shared_from_this<Actor>
	{
	public:
		// Constructors
		Actor();
		Actor(const ObjectInit& _object_init);

		// Destructors
		~Actor() override = default;

		// Reflection
		void Serialize(YAML::Emitter& _out) override;
		void Deserialize(const YAML::Node& _in) override;

		void AddComponent(const Ref<Component>& _comp)
		{
			if (!_comp) return;

			m_Components.push_back(_comp);
			m_ComponentMap[_comp->GetClassNameDE()] = _comp;
		}
		
		template<class T, typename ... Args>
		Ref<T> AddComponent(Args&& ... _args)
		{
			if(Ref<T> comp = MakeRef<T>(std::forward<Args>(_args)...))
			{
				AddComponent(comp);
				return comp;
			}

			return nullptr;
		}

		template<typename T>
		Ref<T> GetComponent() const
		{
			if (!m_ComponentMap.contains(ReflectionHelper::GetClassNameDE<T>()))
				return nullptr;

			if (Ref<T> component = CastRef<T>(m_ComponentMap.at(ReflectionHelper::GetClassNameDE<T>())))
				return component;

			return nullptr;
		}

		// Use with caution
		std::vector<Ref<Component>> &GetComponents()  { return m_Components; }
		std::unordered_map<std::string, Ref<Component>>& GetComponentMap() { return m_ComponentMap; }

		
		Ref<TransformComponent> GetTransformComponent() { return m_TransformComponent; }
		Transform& GetTransform() { return m_TransformComponent->GetTransform(); }
		Transform GetTransform() const { return m_TransformComponent->GetTransform(); }
		
		Ref<PhysicsComponent> GetPhysicsComponent() { return m_PhysicsComponent; }
		Ref<ModelComponent> GetMeshComponent() { return m_ModelComponent; }
		Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

		// Physics Component
		virtual void OnCollisionEnter(const Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point){}
		virtual void OnCollisionExit(const Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point){}

		virtual void OnTriggerEnter(Ref<Actor> _other){}
		virtual void OnTriggerStay(Ref<Actor> _other){}
		virtual void OnTriggerExit(Ref<Actor> _other){}

		virtual void Destroy()
		{
			// Add more clean up code here
			MarkRubbish();
		}

	protected:
		void BeginScene() override
		{
			BaseObject::BeginScene();

			for (const auto& component : m_Components)
			{
				component->m_Parent = shared_from_this();
				component->BeginScene();
			}
		}
		
		void EndScene() override
		{
			for (const auto& component : m_Components) component->EndScene();

			BaseObject::EndScene();
		}

		void BeginPlay() override;
		void EndPlay() override;

		void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
		
		std::unordered_map<std::string, Ref<Component>> m_ComponentMap;
		std::vector<Ref<Component>> m_Components;

		Ref<TransformComponent> m_TransformComponent;

		Ref<PhysicsComponent> m_PhysicsComponent;

		Ref<ModelComponent> m_ModelComponent;

		Ref<RenderComponent> m_RenderComponent;

		friend class Scene;
		friend class SceneSubsystem;

		friend class PhysicsComponent;
		friend class PhysicsSubsystem;
		
		friend class ModelComponent;
		friend class RenderComponent;
		friend class TransformComponent;

		friend class RendererSubsystem;
	};
}
