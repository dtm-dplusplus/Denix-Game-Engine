#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Component.h"
#include "Component.h"
#include "Component.h"
#include "Component.h"
#include "Core.h"

#include "Object.h"

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

	virtual void Initialize() {}
	virtual void Deinitialize() {}
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
	TransformComponent(const GLint _parent_id) : Component(_parent_id, ObjectInitializer("Transform Component")) {}

	// Destructors
	~TransformComponent() override = default;

	void Initialize() override
	{
		//Register with the scene
		//Engine::Get().GetEngineScene()->RegisterTransformComponent(m_ParentObjectID, this);
	}

	void Update() override
	{
		Model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0, -2.5)); // Should translate from camera position
		Model = glm::rotate(Model, glm::degrees(m_Rotation.x), glm::vec3(1, 0, 0));
		Model = glm::rotate(Model, glm::degrees(m_Rotation.y), glm::vec3(0, 1, 0));
		Model = glm::rotate(Model, glm::degrees(m_Rotation.z), glm::vec3(0, 0, 1));
		Model = glm::scale(Model, m_Scale);

		/*Model = glm::translate(glm::mat4(1.0f), CamPos);
		Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
		Model = glm::scale(Model, m_TransformComponent->GetScale());*/

	}

	// Getters
	glm::mat4 GetModel() const { return Model; }

	glm::vec3 GetPosition() const { return m_Position; }
	glm::vec3& GetPosition() { return m_Position; }

	glm::vec3 GetRotation() const { return m_Rotation; }
	glm::vec3& GetRotation() { return m_Rotation; }

	glm::vec3 GetScale() const { return m_Scale; }
	glm::vec3& GetScale() { return m_Scale; }

	// Setters
	void SetPosition(const glm::vec3& _position) { m_Position = _position; }
	void SetRotation(const glm::vec3& _rotation) { m_Rotation = _rotation; }
	void SetScale(const glm::vec3& _scale) { m_Scale = _scale; }
	

protected:
	glm::vec3 m_Position = glm::vec3(0.f);
	glm::vec3 m_Rotation = glm::vec3(0.f);
	glm::vec3 m_Scale = glm::vec3(1.f);

	glm::mat4 Model = glm::mat4(1.f);
};