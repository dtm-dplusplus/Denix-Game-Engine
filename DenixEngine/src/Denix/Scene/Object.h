#pragma once

#include <string>

#include "Denix/Core.h"
#include "GL/glew.h"

namespace Denix
{
	/*
	 * Initialiser for all objects in the game
	 * Only implents the name for now, will be expanded later
	 * e.g. class to construct
	*/
	struct ObjectInit
	{
		// Constructors
		ObjectInit(): Name{ "Object" } {}
		ObjectInit(std::string _name) : Name{std::move(_name)} {}

		std::string Name;
	};

	// Base class for all objects in the game
    class Object
	{
	public:
    	Object() = default;
    	Object(ObjectInit _object_init) :
			m_ID{ CreateNewID() }, m_ObjectData{std::move(_object_init)} {}
    	
		// Destructors
		virtual ~Object() = default;

		GLint GetID() const { return m_ID; }

		std::string GetName() const { return m_ObjectData.Name; }


		bool IsRubbish() const { return m_IsRubbish; }
		void SetIsRubbish() { m_IsRubbish = true; }
	protected:

		/* Name and ID should be hashed in some way in the future */
		/* Unique Identifier */
		GLint m_ID;

		/* Name of the object */
		std::string m_Name;

		/** Clean up value, if marked as rubbish, it will be deleted once the frame has finished*/
		bool m_IsRubbish = false;
	private:

		/* Used to assign object IDs. Represents the tota number of objects created */
		static GLint m_CountID;
		static GLint CreateNewID() { return m_CountID++; }

    	ObjectInit m_ObjectData;
	};
}
