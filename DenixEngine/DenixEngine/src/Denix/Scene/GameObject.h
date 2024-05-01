#pragma once

#include <ranges>

#include "Denix/Scene/Object.h"
#include "Denix/Scene/Component.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Physics/PhysicsComponent.h"
#include "Denix/Video/GL/MeshComponent.h"
#include "Denix/Scene/GameObjectData.h"
#include "Denix/Video/Renderer/RenderComponent.h"

namespace Denix
{
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
		virtual void OnCollide() {} //const Ref<GameObject>& _other

		virtual void OnTriggerEnter() {}
		virtual void OnTriggerStay() {}
		virtual void OnTriggerExit() {}

		Moveability GetMoveability() const { return static_cast<Moveability>(m_Moveability); }
		int& GetMoveability() { return m_Moveability; }
		void SetMoveability(const Moveability _moveability)
		{
			m_Moveability = static_cast<int>(_moveability);
			m_MoveabilityChanged = true;
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
		}

		void LateUpdate(float _deltaTime) override;

	protected:
		/** Moveability defines the complexity of logic performed on object data. It does not explicity determine the object's ability to move.
		* Static is the default value for all objects and uses the least amount of resources.
		* Dynamic is the most complex and uses the most resources.
		* 
		*	@Physics
		*	Static: The object is a stationary collidable. There is no need to sweep or check for collisions
		*	Dynamic: The object is a collidable, affected by external forces and can move. It requires collision detection and resolution.
		* 
		*	@Lighting - (Currently all light sources are dynamic in nature. Static lighting will be supported shortly)
		*	Static: The object is a stationary light source. It does not need to be updated every frame.
		*	Dynamic: The object is a light source that can move. It requires updating every frame.
		*/
		int m_Moveability = static_cast<int>(Moveability::Static);

		// Will be a flag in future
		bool m_MoveabilityChanged = false;

	protected:
		std::unordered_map<std::string, Ref<Component>> m_Components;

		Ref<TransformComponent> m_TransformComponent;

		Ref<PhysicsComponent> m_PhysicsComponent;

		Ref<MeshComponent> m_MeshComponent;

		Ref<RenderComponent> m_RenderComponent;

		friend class SceneSubsystem;
	};

	class Plane : public GameObject
	{
	public:
		Plane(const ObjectInitializer& _objInit = { "Plane" });
	};

	class Cube : public GameObject
	{
	public:
		Cube(const ObjectInitializer& _objInit = { "TestCube" });
	};
}
