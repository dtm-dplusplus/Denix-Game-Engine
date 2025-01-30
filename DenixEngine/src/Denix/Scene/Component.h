#pragma once

#include "Denix/Core/BaseObject.h"

/**
 * @file Component.h
 * @brief 
 */

namespace Denix
{
    class Actor;

    /**
    * @class Component
    * @brief Base class for actor components in the engine.
    *
    * Components are attachable to actors and provide additional functionality.
    */
    class Component : public BaseObject
    {
    public:
        /**
        * @brief Constructs a Component with an optional initializer.
        * @param _objectInitializer Initializer containing component properties.
        */
        Component(const ObjectInit& _objInit = { "Component" }) : BaseObject(_objInit),
            m_IsRemovable(false), m_IsEnabled(true) 
        {
        }

        /**
         * @brief Destructor for the Component.
         */
        ~Component() override = default;

        /**
        * @brief Gets the parent actor of this component.
        * @return Reference to the parent actor.
        */
        Ref<Actor> GetParent() const { return m_Parent.lock(); }

        /**
        * @brief Checks if the component is removable.
        * @return True if removable, false otherwise.
        */
        bool IsRemoveable() const { return m_IsRemovable; }
        bool& IsRemoveable() { return m_IsRemovable; }

        /**
         * @brief Checks if the component is enabled.
         * @return True if enabled, false otherwise.
         */
        bool IsEnabled() const { return m_IsEnabled; }
        bool& IsEnabled() { return m_IsEnabled; }

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

        bool m_IsRemovable;
        bool m_IsEnabled;

        WRef<Actor> m_Parent;

        friend class Actor;
    };
}
