#pragma once

#include "Denix/Core/BaseObject.h"

namespace Denix
{
	class Actor;
	
	/*	Base component class for all Actors  */
	class Component : public BaseObject
	{
	public:
		// Constructors
		Component(const ObjectInit& _objectInitializer = ObjectInit());

		// Destructors
		~Component() override = default;
		
		Ref<Actor> GetParent() const { return m_Parent.lock(); }
		
		void BeginScene() override
		{
			BaseObject::BeginScene();
		}

		void EndScene() override
		{
			BaseObject::EndScene();
		}
		
		void BeginPlay() override
		{
			BaseObject::BeginPlay();
		}
		void EndPlay() override
		{
			BaseObject::EndPlay();
		}
		void Update(float _deltaTime) override
		{
			BaseObject::Update(_deltaTime);
		}

	protected:
		virtual void RegisterComponent() {}
		virtual void UnregisterComponent() {}
		
		WRef<Actor> m_Parent;

		friend class Actor;
		friend class BaseObject;
	};
}