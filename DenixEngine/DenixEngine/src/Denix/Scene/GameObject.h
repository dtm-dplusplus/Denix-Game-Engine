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

	class Triangle : public GameObject
	{
	public:
		Triangle() : GameObject({ "Triangle" })
		{
			const Ref<VertexBuffer> vbo = m_MeshComponent->GetVertexBuffer();
			vbo->Bind(GL_ARRAY_BUFFER);
			vbo->BufferData(GL_ARRAY_BUFFER, sizeof(TriangleData), TriangleData, 3, 6, GL_FLOAT);

			const Ref<VertexArray> vao = m_MeshComponent->GetVertexArray();

			vao->Bind();
			vbo->Bind(vbo->GetTarget());

			// Bind Attribute at Location 0
			vao->AttribPtr(vbo->GetPerPrimitive(), vbo->GetType());

			//// Reset the state
			VertexBuffer::Unbind(GL_ARRAY_BUFFER);
			VertexArray::Unbind();
		}
	};

	class Plane : public GameObject
	{
	public:
		Plane() : GameObject({"Plane"})
		{
			const Ref<VertexBuffer> vbo =m_MeshComponent->GetVertexBuffer();
			vbo->Bind(GL_ARRAY_BUFFER);
			vbo->BufferData(GL_ARRAY_BUFFER, sizeof(PlaneData), PlaneData, 3, 6, GL_FLOAT);

			const Ref<VertexArray> vao = m_MeshComponent->GetVertexArray();

			vao->Bind();
			vbo->Bind(vbo->GetTarget());

			// Bind Attribute at Location 0
			vao->AttribPtr(vbo->GetPerPrimitive(), vbo->GetType());

			//// Reset the state
			VertexBuffer::Unbind(GL_ARRAY_BUFFER);
			VertexArray::Unbind();
		}
	};

	class Cube : public GameObject
	{
	public:
		Cube() : GameObject({ "Cube" })
		{
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
	};
}
