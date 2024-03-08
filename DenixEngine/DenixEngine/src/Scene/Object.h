#pragma once

#include <string>

#include "Core/DeLog.h"
#include "GL/glew.h"

namespace Denix
{
	/*
	 * Initialiser for all objects in the game
	 * Only implents the name for now, will be expanded later
	 * e.g. class to construct
	*/
	struct ObjectInitializer
	{
		// Constructors
		ObjectInitializer() = default;
		ObjectInitializer(std::string _name) : Name{ std::move(_name) } {}

		static ObjectInitializer Get() { return { "Object" }; }

		std::string Name;
	};

	// Base class for all objects in the game
	class Object
	{
	public:
		Object(const ObjectInitializer& _object_init = ObjectInitializer::Get()): m_ID{ 0 }, m_Name{ _object_init.Name }
		{
			// Try to innit object of class (future)
			// if(object instantiated correctly)
			m_ID = CreateNewID();

			// Register object with object manager (future)

			DE_LOG(LogObject, Trace, "Object #{} {} created", m_ID, m_Name.c_str())

			// else
			//DE_LOG(LogObject, Error, "Object {} failed to create", m_Name.c_str())
			// m_ID = -1;
		}

		// Create Object


		// Create Object by class

		// Destructors
		virtual ~Object() = default;

		// Called when the scene is set as active
		virtual void BeginScene() {}
		virtual void EndScene() {}



		virtual void Update(float _deltaTime) {}



		GLint GetID() const { return m_ID; }

		std::string GetName() { return m_Name; }

	protected:

		/* Name and ID should be hashed in some way in the future */
		/* Unique Identifier */
		GLint m_ID;

		/* Name of the object */
		std::string m_Name;

	private:

		/* Used to assign object IDs. Represents the tota number of objects created */
		static GLint m_CountID;
		static GLint CreateNewID() { return m_CountID++; }
	};
}
