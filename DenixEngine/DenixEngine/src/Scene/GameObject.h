#pragma once

#include "Object.h"
#include "Component.h"

/*	Base class for all game objects
*	
*/
class GameObject : public Object
{
public:
	// Constructors
	GameObject(const ObjectInitializer& _object_init = ObjectInitializer::Get()) : Object(_object_init) 
	{
		m_TransformComponent = std::make_shared<TransformComponent>(m_ID);

		m_PhysicsComponent = std::make_shared<PhysicsComponent>(m_ID);

		m_RenderComponent = std::make_shared<RenderComponent>(m_ID);
	}

	// Destructors
	~GameObject() override = default;

	void BeginScene() override
	{
		m_TransformComponent->BeginScene();
		m_PhysicsComponent->BeginScene();
		m_RenderComponent->BeginScene();
	}
	void EndScene() override
	{
	}

	void Update(float _deltaTime) override {}

public:
	Ref<TransformComponent> GetTransformComponent() { return m_TransformComponent; }

	Ref<PhysicsComponent> GetPhysicsComponent() { return m_PhysicsComponent; }

	Ref<RenderComponent> GetRenderComponent() { return m_RenderComponent; }

protected:
	Ref<TransformComponent> m_TransformComponent;

	Ref<PhysicsComponent> m_PhysicsComponent;

	Ref<RenderComponent> m_RenderComponent;
};
