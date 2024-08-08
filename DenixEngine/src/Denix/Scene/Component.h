#pragma once

#include "Object.h"

namespace Denix
{
	/*	Base class for all object components */
	class Component : public Object
	{
	public:
		// Constructors
		Component(const ObjectInitializer& _objectInitializer = ObjectInitializer::Get()) : Object(_objectInitializer)
		{
		}

		Component(const std::string& _parentName, const ObjectInitializer& _objectInitializer = ObjectInitializer::Get()) : Object(_objectInitializer), m_ParentObjectName{ _parentName }
		{
		}

		// Destructors
		~Component() override = default;

		virtual void RegisterComponent() {}
		virtual void UnregisterComponent() {}

	public: // Component Interface
		/** Get the parent object id */
		std::string GetParentObjectName() const { return m_ParentObjectName; }

		/** Set the parent object id */
		void SetParentObjectName(const std::string& _name) { m_ParentObjectName = _name; }

	public: // Object Interface
		virtual void Initialize() {}
		virtual void Deinitialize() {}

		void BeginScene() override {}
		void EndScene() override {}
		void Update(float _deltaTime) override {}

	protected:
		/* Object this component is attacthed to */
		std::string m_ParentObjectName;
	};
}