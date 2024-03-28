#pragma once

#include <ranges>

#include "Object.h"
#include "Component.h"
#include "Component/MeshComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Component/ColliderComponent.h"
#include "MeshData.h"
#include "Denix/Scene/GameObjectData.h"

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
			m_PhysicsComponent->m_ActorTransform = m_TransformComponent;
			m_PhysicsComponent->m_ColliderComponent->m_ActorTransformComponent = m_TransformComponent;
			m_Components["Physics"] = m_PhysicsComponent;

			m_MeshComponent = MakeRef<MeshComponent>(m_ID);
			m_Components["Mesh"] = m_MeshComponent;

			m_RenderComponent = MakeRef<RenderComponent>(m_ID);
			m_Components["Render"] = m_RenderComponent;
		}

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

		Ref<ColliderComponent> GetColliderComponent() const { return m_PhysicsComponent->m_ColliderComponent; }

		Ref<MeshComponent> GetMeshComponent() { return m_MeshComponent; }

		Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

		// Physics Component
		virtual void OnCollide() {} //const Ref<GameObject>& _other
		virtual void OnTrigger() {} // const Ref<GameObject>& _other

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

		void LateUpdate(float _deltaTime) override
		{
			Object::LateUpdate(_deltaTime);

			if (m_MoveabilityChanged)
			{
				// Propogate update to physics systems
				m_PhysicsComponent->SetMoveability(static_cast<Moveability>(m_Moveability));
				m_MoveabilityChanged = false;
			}
		}
	protected:
		/** Moveability Defines many behaviours of the game objects components
		*	@Physics
		*	Static: The object is a stationary collidable and is not affected by external forces
		*	Dynamic: The object is a collidable and is affected by external forces
		*
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

	class EqualTriangle : public GameObject
	{
	public:
		EqualTriangle() : GameObject({ "EqualTriangle" })
		{
			const Ref<VertexBuffer> vbo = m_MeshComponent->GetVertexBuffer();
			vbo->Bind();
			vbo->VertexBufferData(sizeof(EqualTriangleData), EqualTriangleData, 3, 3, GL_FLOAT);

			const Ref<IndexBuffer> ibo = m_MeshComponent->GetIndexBuffer();
			ibo->Bind();
			ibo->IndexBufferData(sizeof(EqualTriangleIndices), EqualTriangleIndices, 3,GL_UNSIGNED_INT);

			// Setup Vertex Array
			const Ref<VertexArray> vao = m_MeshComponent->GetVertexArray();
			vao->Bind();

			// Bind Vertex Buffer at Location 0
			vbo->Bind();
			vao->AttribPtr(vbo->GetPerPrimitive(), vbo->GetType());

			// Bind Index Buffer at Location 1
			ibo->Bind();
			vao->AttribPtr(ibo->GetIndexCount(), ibo->GetType());

			//// Reset the state
			VertexBuffer::Unbind();
			VertexArray::Unbind();
			IndexBuffer::Unbind();
		}
	};

	class RightTriangle : public GameObject
	{
	public:
		RightTriangle() : GameObject({ "RightTriangle" })
		{
			const Ref<VertexBuffer> vbo = m_MeshComponent->GetVertexBuffer();
			vbo->Bind();
			vbo->VertexBufferData(sizeof(RightTriangleData), RightTriangleData, 3, 3, GL_FLOAT);

			const Ref<IndexBuffer> ibo = m_MeshComponent->GetIndexBuffer();
			ibo->Bind();
			ibo->IndexBufferData(sizeof(RightTriangleIndices), RightTriangleIndices, 3, GL_UNSIGNED_INT);

			// Setup Vertex Array
			const Ref<VertexArray> vao = m_MeshComponent->GetVertexArray();
			vao->Bind();

			// Bind Vertex Buffer at Location 0
			vbo->Bind();
			vao->AttribPtr(vbo->GetPerPrimitive(), vbo->GetType());

			// Bind Index Buffer at Location 1
			ibo->Bind();
			vao->AttribPtr(ibo->GetIndexCount(), ibo->GetType());

			//// Reset the state
			VertexBuffer::Unbind();
			VertexArray::Unbind();
			IndexBuffer::Unbind();
		}
	};

	class Plane : public GameObject
	{
	public:
		Plane() : GameObject({"Plane"})
		{
			const Ref<VertexBuffer> vbo =m_MeshComponent->GetVertexBuffer();
			vbo->Bind();
			vbo->VertexBufferData(sizeof(PlaneData), PlaneData, 3, 4, GL_FLOAT);

			const Ref<IndexBuffer> ibo = m_MeshComponent->GetIndexBuffer();
			ibo->Bind();
			ibo->IndexBufferData(sizeof(PlaneIndices), PlaneIndices, 6, GL_UNSIGNED_INT);

			// Setup Vertex Array
			const Ref<VertexArray> vao = m_MeshComponent->GetVertexArray();
			vao->Bind();

			// Bind Vertex Buffer at Location 0
			vbo->Bind();
			vao->AttribPtr(vbo->GetPerPrimitive(), vbo->GetType());

			// Bind Index Buffer at Location 1
			ibo->Bind();
			vao->AttribPtr(ibo->GetIndexCount(), ibo->GetType());

			//// Reset the state
			VertexBuffer::Unbind();
			VertexArray::Unbind();
			IndexBuffer::Unbind();
		}
	};

	class Cube : public GameObject
	{
	public:
		Cube() : GameObject({ "Cube" })
		{
			const Ref<VertexBuffer> vbo = m_MeshComponent->GetVertexBuffer();
			vbo->Bind();
			vbo->VertexBufferData(sizeof(CubeData), CubeData, 3, 8, GL_FLOAT);

			const Ref<IndexBuffer> ibo = m_MeshComponent->GetIndexBuffer();
			ibo->Bind();
			ibo->IndexBufferData(sizeof(CubeIndices), CubeIndices, 36, GL_UNSIGNED_INT);

			// Setup Vertex Array
			const Ref<VertexArray> vao = m_MeshComponent->GetVertexArray();
			vao->Bind();

			// Bind Vertex Buffer at Location 0
			vbo->Bind();
			vao->AttribPtr(vbo->GetPerPrimitive(), vbo->GetType());

			// Bind Index Buffer at Location 1
			ibo->Bind();
			vao->AttribPtr(ibo->GetIndexCount(), ibo->GetType());

			//// Reset the state
			VertexBuffer::Unbind();
			VertexArray::Unbind();
			IndexBuffer::Unbind();
		}
	};
}
