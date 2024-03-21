#pragma once

#include <ranges>

#include "Denix/Core.h"

#include "Denix/Scene/Component.h"
#include "Denix/Scene/Component/MeshComponent.h"
#include "Denix/Scene/Component/RenderComponent.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Scene/MeshData.h"
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

	class ColliderComponent : public Component
	{
	public:
		ColliderComponent() : Component(ObjectInitializer("Collider Component"))
		{
			m_RenderComponent = MakeRef<RenderComponent>(m_ID);
			m_RenderComponent->SetDrawMode(GL_LINES);
			m_RenderComponent->SetIsVisible(false);
			m_Components["Render"] = m_RenderComponent;

			m_MeshComponent = MakeRef<MeshComponent>(m_ID);
			m_Components["Mesh"] = m_MeshComponent;

			const Ref<VertexBuffer> vbo = m_MeshComponent->GetVertexBuffer();
			vbo->Bind(GL_ARRAY_BUFFER);
			vbo->BufferData(GL_ARRAY_BUFFER, sizeof(CubeData), CubeData, 3, 36, GL_FLOAT);

			const Ref<VertexArray> vao = m_MeshComponent->GetVertexArray();

			vao->Bind();
			vbo->Bind(vbo->GetTarget());

			// Bind Attribute at Location 0
			vao->AttribPtr(vbo->GetPerPrimitive(), vbo->GetType());

			//// Reset the state
			VertexBuffer::Unbind(GL_ARRAY_BUFFER);
			VertexArray::Unbind();
		}

		ColliderComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Collider Component"))
		{
			m_RenderComponent = MakeRef<RenderComponent>(m_ID);
			m_RenderComponent->SetDrawMode(GL_LINES);
			m_RenderComponent->SetDebugColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			m_RenderComponent->SetIsVisible(false);
			m_Components["Render"] = m_RenderComponent;

			m_MeshComponent = MakeRef<MeshComponent>(m_ID);
			m_Components["Mesh"] = m_MeshComponent;

			const Ref<VertexBuffer> vbo = m_MeshComponent->GetVertexBuffer();
			vbo->Bind(GL_ARRAY_BUFFER);
			vbo->BufferData(GL_ARRAY_BUFFER, sizeof(CubeData), CubeData, 3, 36, GL_FLOAT);

			const Ref<VertexArray> vao = m_MeshComponent->GetVertexArray();

			vao->Bind();
			vbo->Bind(vbo->GetTarget());

			// Bind Attribute at Location 0
			vao->AttribPtr(vbo->GetPerPrimitive(), vbo->GetType());

			//// Reset the state
			VertexBuffer::Unbind(GL_ARRAY_BUFFER);
			VertexArray::Unbind();
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

		/** Transform component which is attached to this components game object */
		Ref<TransformComponent> m_ActorTransformComponent;

		friend class SceneSubsystem;
		friend class PhysicsSubsystem;
		friend class GameObject;
		friend class PhysicsComponent;
	};
}
