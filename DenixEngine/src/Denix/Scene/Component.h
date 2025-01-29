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

        bool IsRemoveable() const { return m_IsRemovable; }

    protected:
        virtual void RegisterComponent()
        {
        }

        virtual void UnregisterComponent()
        {
        }

        virtual void OnComponentAdded()
        {
        }

        virtual void OnComponentRemoved()
        {
        }

        bool m_IsRemovable = false;

        WRef<Actor> m_Parent;

        friend class Actor;
        friend class BaseObject;
    };
}
