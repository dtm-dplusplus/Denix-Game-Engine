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

		template<class T, typename ... Args>
		Ref<T> AddComponent(Args&& ... _args)
		{
			if(Ref<T> component = MakeRef<T>(std::forward<Args>(_args)...))
			{
				component->m_Parent = this;
				m_Components.push_back(component);
				m_ComponentMap[ReflectionHelper::GetDEClassName<T>()] = component;
				return component;
			}

			return nullptr;
		}

		template<typename T>
		Ref<T> GetComponent() const
		{
			if (!m_ComponentMap.contains(ReflectionHelper::GetDEClassName<T>()))
				return nullptr;

			if (Ref<T> component = CastRef<T>(m_ComponentMap.at(ReflectionHelper::GetDEClassName<T>())))
				return component;

			return nullptr;
		}

		Ref<TransformComponent> GetTransformComponent() { return m_TransformComponent; }

		Ref<PhysicsComponent> GetPhysicsComponent() { return m_PhysicsComponent; }
		Ref<Collider> GetCollider() const { return m_PhysicsComponent->GetCollider(); }

		Ref<MeshComponent> GetMeshComponent() { return m_MeshComponent; }

		Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

		// Physics Component
		virtual void OnCollision(Ref<Actor>& _other, CollisionData& _collision) {} //const Ref<Actor>& _other

		virtual void OnTriggerEnter(Ref<Actor> _other);
		virtual void OnTriggerStay(Ref<Actor> _other);
		virtual void OnTriggerExit(Ref<Actor> _other);

		void Destroy();

	public:
		void BeginScene() override;
		void EndScene() override;

		void BeginPlay() override;
		void EndPlay() override;

		void Update(float _deltaTime) override;

	protected:
		std::unordered_map<std::string, Ref<Component>> m_ComponentMap;
		std::vector<Ref<Component>> m_Components;

		Ref<TransformComponent> m_TransformComponent;

		Ref<PhysicsComponent> m_PhysicsComponent;

		Ref<MeshComponent> m_MeshComponent;

		Ref<RenderComponent> m_RenderComponent;

		friend class SceneSubsystem;
		friend class Scene;
		friend class RendererSubsystem;
	};
}
