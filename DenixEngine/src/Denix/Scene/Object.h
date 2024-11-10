#pragma once

#include <string>

#include "Denix/Core.h"

namespace Denix
{
	/*
	 * Initialiser for all objects in the game
	 * Only implents the name for now, it will be expanded in the future
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
    	Object()
    	{
    		m_GUID = GenID();
    		m_IsRubbish = false;
    		m_Name = ObjectInit().Name;
    	}
    	
    	Object(const ObjectInit& _object_init)
    	{
    		m_GUID = GenID();
    		m_IsRubbish = false;
    		m_Name = _object_init.Name;
    	}
    	
		// Destructors
		virtual ~Object() = default;

	    /**
		 * 
		 * @return The name of the object
		 */
		std::string GetName() const { return m_Name; }

		/**
		 * Global Unique Identifier 
		 * @return The GUID of the object
		 */
    	unsigned int GetGUID() const { return m_GUID; }
    	
		bool IsRubbish() const { return m_IsRubbish; }

    protected:
    	/**
		 * Set the name of the object.
		 * Use with caution, the name should be validated before setting
		 * @param _name The name of the object 
		 */
    	void SetName(const std::string& _name) { m_Name = _name; }

	    /**
    	 * Mark the object as rubbish, it will be deleted at the end of the frame
    	 * Use with caution, the object should be validated before marking.
    	 */
    	void MarkRubbish() { m_IsRubbish = true; }

    	
    private:
		/* Name and ID should be hashed in some way in the future */
		/* Unique Identifier */
		unsigned int m_GUID;

		/* Name of the object */
		std::string m_Name;

		/** Clean up value, if marked as rubbish, it will be deleted once the frame has finished*/
		bool m_IsRubbish;

		/* Used to assign object IDs. Represents the tota number of objects created */
		static unsigned int GenID();
	};
}
