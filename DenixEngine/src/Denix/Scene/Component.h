#pragma once

#include "BaseObject.h"

namespace Denix
{
	class Actor;
	
	/*	Base component class for all Actors  */
	class Component : public BaseObject
	{
	public:
		// Constructors
		Component(const ObjectInit& _objectInitializer = ObjectInit()) : BaseObject(_objectInitializer)
		{
		}

		// Destructors
		~Component() override = default;
		

	public: // Object Interface
		void BeginScene() override
		{
			BaseObject::BeginScene();
		}
		void BeginPlay() override
		{
			BaseObject::BeginPlay();
		}
		void EndPlay() override
		{
			BaseObject::EndPlay();
		}
		void EndScene() override
		{
			BaseObject::EndScene();
		}
		void Update(float _deltaTime) override
		{
			BaseObject::Update(_deltaTime);
		}

	protected:
		virtual void RegisterComponent() {}
		virtual void UnregisterComponent() {}
		
		Ref<Actor> m_Parent;

		friend class Actor;
		friend class BaseObject;
	};
}