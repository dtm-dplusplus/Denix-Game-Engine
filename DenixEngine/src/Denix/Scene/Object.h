#pragma once

#include <string>

#include "Denix/Core.h"
#include "Denix/Core/Logger.h"
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
		ObjectInitializer(): Name{ "Object" } {}
		ObjectInitializer(std::string _name) : Name{std::move(_name)} {}

		static ObjectInitializer Get() { return { "Object" }; }

		std::string Name;

	};

	// Base class for all objects in the game
    class Object
	{
	public:
    	Object() = default;
    	Object(const ObjectInitializer& _object_init) :
			m_ID{ CreateNewID() },
			m_FriendlyName{ _object_init.Name },
			m_Name{ std::to_string(m_ID) + "_" + m_FriendlyName } {}
    	
		/*Object(const Object& _other)
			: m_ID(_other.m_ID),
			  m_FriendlyName(_other.m_FriendlyName),
			  m_Name(_other.m_Name),
			  m_IsRubbish(_other.m_IsRubbish)
		{
		}

		Object(Object&& _other) noexcept
			: m_ID(_other.m_ID),
			  m_FriendlyName(std::move(_other.m_FriendlyName)),
			  m_Name(std::move(_other.m_Name)),
			  m_IsRubbish(_other.m_IsRubbish)
		{
		}

		Object& operator=(const Object& _other)
		{
			if (this == &_other)
				return *this;
			m_ID = _other.m_ID;
			m_FriendlyName = _other.m_FriendlyName;
			m_Name = _other.m_Name;
			m_IsRubbish = _other.m_IsRubbish;
			return *this;
		}

		Object& operator=(Object&& _other) noexcept
		{
			if (this == &_other)
				return *this;
			m_ID = _other.m_ID;
			m_FriendlyName = std::move(_other.m_FriendlyName);
			m_Name = std::move(_other.m_Name);
			m_IsRubbish = _other.m_IsRubbish;
			return *this;
		}*/

		// Create Object
		static Ref<Object> Create()
		{
			return MakeRef<Object>();
		}

		static Ref<Object> Create(const ObjectInitializer& _object_init = ObjectInitializer::Get())
		{
			return MakeRef<Object>(_object_init);
		}

		// Destructors
		virtual ~Object() = default;

		GLint GetID() const { return m_ID; }

		std::string GetName() { return m_Name; }

		std::string GetFriendlyName() { return m_FriendlyName; }

		bool IsRubbish() const { return m_IsRubbish; }
		void SetIsRubbish() { m_IsRubbish = true; }
	protected:

		/* Name and ID should be hashed in some way in the future */
		/* Unique Identifier */
		GLint m_ID;

		/* Friendly name of the object displayed to user */
		std::string m_FriendlyName;

		/* Name of the object */
		std::string m_Name;

		/** Clean up value, if marked as rubbish, it will be deleted once the frame has finished*/
		bool m_IsRubbish = false;
	private:

		/* Used to assign object IDs. Represents the tota number of objects created */
		static GLint m_CountID;
		static GLint CreateNewID() { return m_CountID++; }
	};
}
