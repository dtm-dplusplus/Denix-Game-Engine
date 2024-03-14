#pragma once

#include "Object.h"

namespace Denix
{
	/*	Base class for all object components */
	class Component : public Object
	{
	public:
		// Constructors
		Component(const ObjectInitializer& _objectInitializer = ObjectInitializer::Get()) : Object(_objectInitializer), m_ParentObjectID{ 0 }
		{
		}

		Component(const GLint _parentID, const ObjectInitializer& _objectInitializer = ObjectInitializer::Get()) : Object(_objectInitializer), m_ParentObjectID{ _parentID }
		{
		}

		// Destructors
		~Component() override = default;

		virtual void RegisterComponent() {}
		virtual void UnregisterComponent() {}

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
}