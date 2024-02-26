#pragma once

#include <string>

#include "Core/DeLog.h"
#include "GL/glew.h"

/*
 * Initialiser for all objects in the game
 * Only implents the name for now, will be expanded later
 * e.g. class to construct
*/
struct ObjectInitializer
{
	ObjectInitializer(std::string _name) : Name{ std::move(_name) } {}

	static ObjectInitializer Get() { return { "Object" }; }

	std::string Name;
};

// Base class for all objects in the game
class Object
{
public:
	// Constructors
	Object(const ObjectInitializer& _object_init = ObjectInitializer::Get()): m_ID{ 0 }, m_Name{ _object_init.Name }
	{
		// Try to innit object of class (future)
		// if(object instantiated correctly)
		m_ID = CreateNewID();

		// Register object with object manager (future)

		DE_LOG(LogObject, Info, "Object {} created", m_Name.c_str())

		// else
		//DE_LOG(LogObject, Error, "Object {} failed to create", m_Name.c_str())
	}

	// Create Object


	// Create Object by class

	// Destructors
	virtual ~Object() = default;

	virtual void Update() {}


	GLuint GetID() const { return m_ID; }

	void SetName(const std::string& _name) { m_Name = _name; }
	std::string GetName() { return m_Name; }

protected:

	/* Name and ID should be hashed in some way in the future */
	/* Unique Identifier */
	GLuint m_ID;

	/* Name of the object */
	std::string m_Name;

private:

	/* Used to assign object IDs. Represents the tota number of objects created */
	static GLuint m_CountID;
	static GLuint CreateNewID() { return m_CountID++; }
};


/* Base class for all game objects
 * Game Objects always have Scene components
 *  They only exist during the lifetime of the scene
*/

