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
	class Actor : public BaseObject
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

		void AddComponent(const Ref<Component>& _comp);

		template<class T, typename ... Args>
		Ref<T> AddComponent(Args&& ... _args);

		template<typename T>
		Ref<T> GetComponent() const;

		// Use with caution
		std::vector<Ref<Component>> &GetComponents()  { return m_Components; }
		std::unordered_map<std::string, Ref<Component>>& GetComponentMap() { return m_ComponentMap; }
		
		Ref<TransformComponent> GetTransformComponent() { return m_TransformComponent; }
		Transform& GetTransform() { return m_TransformComponent->GetTransform(); }
		Transform GetTransform() const { return m_TransformComponent->GetTransform(); }
		
		Ref<PhysicsComponent> GetPhysicsComponent() { return m_PhysicsComponent; }
		Ref<ModelComponent> GetModelComponent() { return m_ModelComponent; }
		Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

		void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

		void Destroy() override;
	protected:
		void BeginScene() override;

		void EndScene() override;

		void BeginPlay() override;

		void EndPlay() override;

		// Physics Component
		virtual void OnCollisionEnter(const Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point){}
		virtual void OnCollisionExit(const Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point){}


		Ref<TransformComponent> m_TransformComponent;

		Ref<PhysicsComponent> m_PhysicsComponent;

		Ref<ModelComponent> m_ModelComponent;

		Ref<RenderComponent> m_RenderComponent;
		
	private:
		std::unordered_map<std::string, Ref<Component>> m_ComponentMap;
		std::vector<Ref<Component>> m_Components;

		WRef<Scene> m_SceneRef;
		
		friend class Scene;
		
		friend class SceneSubsystem;
		friend class PhysicsSubsystem;
		friend class RendererSubsystem;

		friend class TransformComponent;
		friend class ModelComponent;
		friend class RenderComponent;
		friend class PhysicsComponent;

	};

	template <class T, typename ... Args>
	Ref<T> Actor::AddComponent(Args&&... _args)
	{
		if(Ref<T> comp = MakeRef<T>(std::forward<Args>(_args)...))
		{
			AddComponent(comp);
			return comp;
		}

		return nullptr;
	}

	template <typename T>
	Ref<T> Actor::GetComponent() const
	{
		if (!m_ComponentMap.contains(ReflectionHelper::GetClassNameDE<T>()))
			return nullptr;

		if (Ref<T> component = CastRef<T>(m_ComponentMap.at(ReflectionHelper::GetClassNameDE<T>())))
			return component;

		return nullptr;
	}
}
