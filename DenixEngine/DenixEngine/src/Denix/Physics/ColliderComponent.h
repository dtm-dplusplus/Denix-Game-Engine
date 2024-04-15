#pragma once

#include <ranges>

#include "Denix/Core.h"

#include "Denix/Scene/Component.h"
#include "Denix/Scene/GameObject.h"

#include "Denix/Video/GL/MeshComponent.h"
#include "Denix/Video/Renderer/RenderComponent.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Scene/GameObjectData.h"

namespace Denix
{
	struct CollisionEvent
	{
		Ref<class ColliderComponent> ColliderA;
		Ref<class ColliderComponent> ColliderB;

		glm::vec3 Normal;
		glm::vec3 ContactPoint;
	};

	enum class ColliderType
	{
		Plane,
		Cube,
		Sphere
	};

	/** Colliders act as basic volumes which can be scaled about their origin */
	class Collider : public GameObject
	{
	public:
		Collider() : GameObject(ObjectInitializer("Collider"))
		{
		}

		Collider(const ObjectInitializer& _objInit = { "Collider" }) : GameObject(ObjectInitializer("Collider"))
		{
		}

		~Collider() override
		{
		}

		bool IsColliding() const { return m_IsColliding; }
		void ToggleColliding()
		{
			m_IsColliding = !m_IsColliding;
			glm::vec4& color = m_RenderComponent->GetDebugColor();
			color = m_IsColliding ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		}

	private:
		Moveability m_ParentMoveability = Moveability::Static;

		bool m_IsColliding = false;

		/** Mesh object that defines the collider collision space */
		Ref<Mesh> m_Mesh;

		/** Render component that is used to draw the collider */
		const static glm::vec4 m_NoCollisionColor;
		const static glm::vec4 m_CollisionColor;

		friend class SceneSubsystem;
		friend class PhysicsSubsystem;
		friend class GameObject;
		friend class PhysicsComponent;
	};

	class ColliderComponent : public Component
	{
	public:
		ColliderComponent() : Component(ObjectInitializer("Collider Component"))
		{
			m_RenderComponent = MakeRef<RenderComponent>();
			m_RenderComponent->SetDebugColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			m_RenderComponent->SetIsVisible(false);
			m_Components["Render"] = m_RenderComponent;

			m_MeshComponent = MakeRef<MeshComponent>();
			m_Components["Mesh"] = m_MeshComponent;
		}

		ColliderComponent(const std::string& _parentName) : Component(_parentName, ObjectInitializer("Collider Component"))
		{
			m_RenderComponent = MakeRef<RenderComponent>(_parentName);
			m_RenderComponent->SetDebugColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			m_RenderComponent->SetIsVisible(false);
			m_Components["Render"] = m_RenderComponent;

			m_MeshComponent = MakeRef<MeshComponent>(_parentName);
			m_Components["Mesh"] = m_MeshComponent;
		}

		~ColliderComponent() override
		{
			m_Components.clear();
		}

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

		bool IsColliding() const { return m_IsColliding; }
		void ToggleColliding() 
		{
			m_IsColliding = !m_IsColliding;
			glm::vec4& color = m_RenderComponent->GetDebugColor();
			color = m_IsColliding ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		}


	public:
		Ref<MeshComponent> GetMeshComponent() { return m_MeshComponent; }

		Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

	private:
		Moveability m_Moveability = Moveability::Static;

		bool m_IsColliding = false;
		
		std::unordered_map<std::string, Ref<Component>> m_Components;

		/** Mesh object that defines the collider collision space */
		Ref<MeshComponent> m_MeshComponent;

		/** Render component that is used to draw the collider */
		Ref<RenderComponent> m_RenderComponent;
		const static glm::vec4 m_NoCollisionColor;
		const static glm::vec4 m_CollisionColor;

		/** Transform component which is attached to this components game object */
		Ref<TransformComponent> m_ActorTransformComponent;

		friend class SceneSubsystem;
		friend class PhysicsSubsystem;
		friend class GameObject;
		friend class PhysicsComponent;
	};
}