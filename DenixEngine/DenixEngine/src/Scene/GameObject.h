#pragma once

#include "Object.h"
#include "Component.h"

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

			m_PhysicsComponent = MakeRef<PhysicsComponent>(m_ID);

			m_MeshComponent = MakeRef<MeshComponent>(m_ID);

			m_RenderComponent = MakeRef<RenderComponent>(m_ID);

		}

		// Destructors
		~GameObject() override = default;

		void BeginScene() override
		{
			m_TransformComponent->BeginScene();
			m_PhysicsComponent->BeginScene();
			m_MeshComponent->BeginScene();
			m_RenderComponent->BeginScene();

		}
		void EndScene() override
		{
		}

		void Update(float _deltaTime) override {}

		Ref<TransformComponent> GetTransformComponent() { return m_TransformComponent; }

		Ref<PhysicsComponent> GetPhysicsComponent() { return m_PhysicsComponent; }

		Ref<MeshComponent> GetMeshComponent() { return m_MeshComponent; }

		Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

	protected:
		Ref<TransformComponent> m_TransformComponent;

		Ref<PhysicsComponent> m_PhysicsComponent;

		Ref<MeshComponent> m_MeshComponent;

		Ref<RenderComponent> m_RenderComponent;
	};
}
