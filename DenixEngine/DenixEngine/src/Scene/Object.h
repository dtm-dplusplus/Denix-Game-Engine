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
	ObjectInitializer() = default;
	ObjectInitializer(std::string _name) : Name{std::move(_name)} {}

	static ObjectInitializer Get() { return {"Object"}; }
	std::string Name;
};

// Base class for all objects in the game
class Object
{
public:
	// Constructors
	Object() = default;

	Object(const ObjectInitializer& _object_init = ObjectInitializer::Get()) : m_ID{0}, m_Name{ _object_init.Name }
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

	virtual void Update() = 0;


	void SetName(const std::string& _name) { m_Name = _name; }
	std::string GetName() { return m_Name; }

protected:

	GLuint m_ID;
	std::string m_Name;

private:

	/* Used to assign object IDs */
	static GLuint m_CountID;
	static GLuint CreateNewID() { return m_CountID++; }
};


/* Base class for all game objects
 * Game Objects always have Scene components
 *  They only exist during the lifetime of the scene
*/ 
class GameObject : public Object
{
public:
		// Constructors
		GameObject() = default;
		GameObject(const ObjectInitializer& _object_init = ObjectInitializer::Get()) : Object(_object_init) {}

		// Destructors
		~GameObject() override = default;

		void Update() override {}
		virtual void Draw() = 0;
};