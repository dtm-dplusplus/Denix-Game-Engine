#pragma once

#include "Scene/Object.h"

class GameObject : public Object
{
public:
	// Constructors
	GameObject(const ObjectInitializer& _object_init = ObjectInitializer::Get()) : Object(_object_init) {}

	// Destructors
	~GameObject() override = default;

	void Update() override {}
};
