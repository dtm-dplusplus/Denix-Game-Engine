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
	}

	// Destructors
	~GameObject() override = default;

	void Update() override {}

	// Will be moved to a renderer class
	virtual void Draw() {}
protected:
	Ref<TransformComponent> m_TransformComponent;
};
