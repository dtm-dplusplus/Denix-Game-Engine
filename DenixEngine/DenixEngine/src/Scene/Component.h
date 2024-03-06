#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Core.h"

#include "Object.h"


/*	Base class for all object components */
class Component: public Object
{
public:
	// Constructors
	Component(const ObjectInitializer& _objectInitializer = ObjectInitializer::Get()) : Object(_objectInitializer), m_ParentObjectID{0}
	{
		DE_LOG(LogObject, Info, "Created Component: {} without Parent Object", m_Name)
	}

	Component(const GLint _parentID, const ObjectInitializer& _objectInitializer = ObjectInitializer::Get()) : Object(_objectInitializer), m_ParentObjectID{ _parentID }
	{
		DE_LOG(LogObject, Info, "Created Component: {} with Parent Object ID: {}", m_Name, m_ParentObjectID)
	}

	// Destructors
	~Component() override = default;

public: // Component Interface
	/** Get the parent object id */
	GLint GetParentObjectID() const { return m_ParentObjectID; }

	/** Set the parent object id */
	void SetParentObjectID(const GLint _id) { m_ParentObjectID = _id; }

public: // Object Interface
	virtual void Initialize() {}
	virtual void Deinitialize() {}

	void BeginScene() override {}
	void EndScene() override {}
	void Update(float _deltaTime) override {}

protected:
	/* Object this component is attacthed to */
	GLint m_ParentObjectID;
};

class TransformComponent : public Component
{
public:
	// Constructors
	TransformComponent() : Component(ObjectInitializer("Transform Component")) {}
	TransformComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Transform Component")) {}

	TransformComponent& operator=(const TransformComponent& _other)
	{
		m_Position = _other.m_Position;
		m_Rotation = _other.m_Rotation;
		m_Scale = _other.m_Scale;

		Model = _other.Model;

		return *this;
	}

	// Destructors
	~TransformComponent() override = default;

	void Initialize() override
	{

	}

	void Deinitialize() override
	{
	}
	void BeginScene() override
	{
		
	}

	void EndScene() override
	{
		m_Position = glm::vec3(0.f);
		m_Rotation = glm::vec3(0.f);
		m_Scale = glm::vec3(1.f);
	}

	void Update(float _deltaTime) override
	{
		Model = glm::translate(glm::mat4(1.0f), m_Position); // Should translate from camera position
		Model = glm::rotate(Model, glm::degrees(m_Rotation.x), glm::vec3(1, 0, 0));
		Model = glm::rotate(Model, glm::degrees(m_Rotation.y), glm::vec3(0, 1, 0));
		Model = glm::rotate(Model, glm::degrees(m_Rotation.z), glm::vec3(0, 0, 1));
		Model = glm::scale(Model, m_Scale);
	}

	// Getters
	glm::mat4 GetModel() const { return Model; }
	glm::mat4& GetModel() { return Model; }

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

class PhysicsComponent : public Component
{
public:
	PhysicsComponent() : Component(ObjectInitializer("Physics Component")) {}
	PhysicsComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Physics Component")) {}

	PhysicsComponent& operator=(const PhysicsComponent& _other)
	{
		m_IsSimulated = _other.m_IsSimulated;
		m_IsCustomGravity = _other.m_IsCustomGravity;

		m_Mass = _other.m_Mass;
		m_Gravity = _other.m_Gravity;
		m_Drag = _other.m_Drag;

		m_Velocity = _other.m_Velocity;
		m_AngularVelocity = _other.m_AngularVelocity;
		m_Acceleration = _other.m_Acceleration;
		m_Force = _other.m_Force;

		return *this;
	}

	// Destructors
	~PhysicsComponent() override = default;

	void Initialize() override
	{
		//Register with the scene
		//Engine::Get().GetEngineScene()->RegisterTransformComponent(m_ParentObjectID, this);
	}

	void BeginScene() override
	{
		
	}

	void EndScene() override
	{
		
	}

	void Update(float _deltaTime) override
	{
		// Clear forces
		m_Force = glm::vec3(0.f);

		// Add acceleration due to gravity
		m_Acceleration += m_Gravity;

		// Compute forces
		m_Force = m_Mass * m_Acceleration;

		// Compute collision response

		// Step integration
		StepEuler(_deltaTime);
	}

public:
	void StepEuler(float _deltaTime)
	{
		// Calculate the net force
		const glm::vec3 netF = m_Force - m_Drag * m_Velocity;

		// Calculate new velocity at time t + dt
		const glm::vec3 vNew = m_Velocity + (netF / m_Mass) * _deltaTime;

		// Calculate new displacement at time t + dt
		const glm::vec3 disp = m_TempPosition + vNew * _deltaTime;

		// Update the object's velocity and position
		m_Velocity = vNew;
		m_TempPosition = disp;
	}

	void StepRK2(float deltaTs){}
	void StepRK4(float deltaTs){}
	void StepVerlet(float deltaTs){}

	void ToggleSimulation()
	{
		m_IsSimulated = !m_IsSimulated;

		if(m_IsSimulated)
		{
			DE_LOG(LogPhysics, Info, "Physics simulation enabled")
		}
		else
		{
			DE_LOG(LogPhysics, Info, "Physics simulation disabled")
		}
	}

	void ToggleGravity()
	{
		m_IsCustomGravity = !m_IsCustomGravity;

		if(m_IsCustomGravity)
		{
			DE_LOG(LogPhysics, Trace, "Custom Gravity enabled")

			// Get the scene gravity (future)
			// Just use the default gravity for now
			m_Gravity = glm::vec3(0.f, -9.81, 0.f);
		}
		else
		{
			DE_LOG(LogPhysics, Trace, "Custom Gravity disabled")
		}
	}
	// Getters
	bool IsSimulated() const { return m_IsSimulated; }
	bool& IsSimulated() { return m_IsSimulated; }

	bool IsCustomGravity() const { return m_IsCustomGravity; }
	bool& IsCustomGravity() { return m_IsCustomGravity; }

	glm::vec3 GetVelocity() const { return m_Velocity; }
	glm::vec3& GetVelocity() { return m_Velocity; }

	glm::vec3 GetAngularVelocity() const { return m_AngularVelocity; }
	glm::vec3& GetAngularVelocity() { return m_AngularVelocity; }

	glm::vec3 GetAcceleration() const { return m_Acceleration; }
	glm::vec3& GetAcceleration() { return m_Acceleration; }

	glm::vec3 GetForce() const { return m_Force; }
	glm::vec3& GetForce() { return m_Force; }

	float GetMass() const { return m_Mass; }
	float& GetMass() { return m_Mass; }

	glm::vec3 GetDrag() const { return m_Drag; }
	glm::vec3& GetDrag() { return m_Drag; }

	glm::vec3 GetGravity() const { return m_Gravity; }
	glm::vec3& GetGravity() { return m_Gravity; }

private:
	/** Set to decide if the physics component should update simulation */
	bool m_IsSimulated = true;

	/** Set to decide if the physics component should use custom gravity
	 * If false, the physics component will use the scene gravity
	 */
	bool m_IsCustomGravity = false;

private:
	/** Mass of the object */
	float m_Mass = 1.0f;

	/** Gravitational forceacting on the object */
	glm::vec3 m_Gravity = glm::vec3(0.f, -9.81, 0.f);

	/** Drag force acting on the object */
	glm::vec3 m_Drag = glm::vec3(0.f);

	/** Velocity of the object */
	glm::vec3 m_Velocity = glm::vec3(0.f);

	/** Angular velocity of the object */
	glm::vec3 m_AngularVelocity = glm::vec3(0.f);

	/** Acceleration of the object */
	glm::vec3 m_Acceleration = glm::vec3(0.f);

	/** Force acting on the object */
	glm::vec3 m_Force = glm::vec3(0.f);

	/** Temporary position of the object
	 * Will be removed in the future
	 */
	glm::vec3 m_TempPosition = glm::vec3(0.f);

	friend class SceneSubSystem;
};

class RenderComponent : public Component
{
public:
	RenderComponent() : Component(ObjectInitializer("Render Component"))
	{
		m_DebugColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.f);
	}

	RenderComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Render Component"))
	{
		m_DebugColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.f);
	}

	~RenderComponent() override = default;

	void Initialize() override
	{
	}

	void Deinitialize() override
	{
	}

	void BeginScene() override{}
	void EndScene() override{}

public:
	glm::vec4 GetDebugColor() const { return m_DebugColor; }
	glm::vec4& GetDebugColor() { return m_DebugColor; }
	glm::vec4 SetDebugColor(const glm::vec4& _color) { return m_DebugColor = _color; }

private:
	glm::vec4 m_DebugColor = glm::vec4(0.2f,0.2f,0.2f, 1.f);
};