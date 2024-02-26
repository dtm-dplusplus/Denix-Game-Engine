#pragma once

#include <glm/vec3.hpp>

#include "Core.h"

#include "Scene/Object.h"

/*	Base class for all object components */
class Component: public Object
{
public:
	// Constructors
	Component(const ObjectInitializer& _object_init = ObjectInitializer::Get()) : Object(_object_init), m_ParentObjectID{0}
	{
		DE_LOG(LogObject, Info, "Created Component: {} without Parent Object", m_Name)
	}

	Component(const GLint _parent_id, const ObjectInitializer& _object_init = ObjectInitializer::Get()) : Object(_object_init), m_ParentObjectID{ _parent_id }
	{
		DE_LOG(LogObject, Info, "Created Component: {} with Parent Object ID: {}", m_Name, m_ParentObjectID)
	}

	// Destructors
	~Component() override = default;

	void Update() override {}

	/* Get the parent object id */
	GLint GetParentObjectID() const { return m_ParentObjectID; }
	void SetParentObjectID(const GLint _id) { m_ParentObjectID = _id; }
protected:
	/* Object this component is attacthed to */
	GLint m_ParentObjectID;
};

class TransformComponent : public Component
{
public:
	TransformComponent() : Component(ObjectInitializer("Transform Component")) {}
	TransformComponent(const GLint _parent_id): Component(_parent_id, ObjectInitializer("Transform Component")) {}

	// Destructors
	~TransformComponent() override = default;

	void Update() override {}

	protected:
		glm::vec3 m_Position = glm::vec3(0.f);
		glm::vec3 m_Rotation = glm::vec3(0.f);
		glm::vec3 m_Scale = glm::vec3(1.f);
};

/*	Base class for all render components */
class RenderComponent : public Component
{
public:
	RenderComponent(): Component(ObjectInitializer("Render Component")) {}
	RenderComponent(const GLint _parent_id) : Component(_parent_id, ObjectInitializer("Render Component")) 
	{
	
	}

	virtual void Draw() {}

protected:
		// Add
};
