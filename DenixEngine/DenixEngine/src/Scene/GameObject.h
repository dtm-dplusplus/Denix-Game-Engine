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

	void Update(float _deltaTime) override {}

	Ref<TransformComponent> GetTransformComponent() { return m_TransformComponent; }


protected:
	Ref<TransformComponent> m_TransformComponent;

};
