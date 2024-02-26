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
		m_RenderComponenet = std::make_shared<RenderComponent>(m_ID);
	}

	// Destructors
	~GameObject() override = default;

	void Update() override {}

protected:
	std::shared_ptr<TransformComponent> m_TransformComponent;
	std::shared_ptr<RenderComponent> m_RenderComponenet;
};
