/**
 * @file Object.h
 * @brief Defines the base Object class and ObjectInit struct for runtime objects.
 */

#pragma once

#include <string>
#include "Denix/Core.h"

namespace Denix
{
    /**
     * @struct ObjectInit
     * @brief Initializer for all runtime objects.
     * 
     * Currently only implements the name but will be expanded in the future.
     */
    struct ObjectInit
    {
        /**
         * @brief Default constructor initializing the object name to "Object".
         */
        ObjectInit(): Name{"Object"} {}

        /**
         * @brief Constructor initializing the object with a specified name.
         * @param _name Name of the object.
         */
        ObjectInit(std::string _name) : Name{std::move(_name)} {}

        /**
         * @brief Name of the object.
         */
        std::string Name;
    };

    /**
     * @class Object
     * @brief Base class for all runtime objects.
     */
    class Object
    {
    public:
        /**
         * @brief Default constructor initializing the object with a generated ID and default name.
         */
        Object(): m_GUID{GenID()}, m_Name{ObjectInit().Name}, m_IsRubbish{false} {}

        /**
         * @brief Constructor initializing the object with a specified initializer.
         * @param _object_init Object initialization parameters.
         */
        Object(const ObjectInit& _object_init): m_GUID{GenID()}, m_Name{_object_init.Name}, m_IsRubbish{false} {}

        /**
         * @brief Virtual destructor.
         */
        virtual ~Object() = default;

        /**
         * @brief Gets the name of the object.
         * @return The name of the object.
         */
        std::string GetName() const { return m_Name; }

        /**
          * @brief Sets the name of the object.
          *
          * Use with caution; the name should be validated before setting.
          * @param _name The new name of the object.
          */
        void SetName(const std::string& _name) { m_Name = _name; }

        /**
         * @brief Gets the class name of the object.
         * @return The class name of the object.
         */
        std::string GetClassNameDE() const { return m_ClassName; }

        /**
         * @brief Gets the Global Unique Identifier (GUID) of the object.
         * @return The GUID of the object.
         */
        size_t GetGUID() const { return m_GUID; }

        /**
         * @brief Checks if the object is marked as rubbish.
         * @return True if the object is marked for deletion, otherwise false.
         */
        bool IsRubbish() const { return m_IsRubbish; }

    protected:
        /**
         * @brief Marks the object as rubbish. It will be deleted at the end of the frame.
         */
        void MarkRubbish() { m_IsRubbish = true; }

        /**
         * @brief Class name of the object.
         */
        std::string m_ClassName;

    private:
        /**
         * @brief Unique Identifier.
         * 
         * Name and ID should be hashed in some way in the future.
         */
        size_t m_GUID;

        /**
         * @brief Name of the object.
         */
        std::string m_Name;

        /**
         * @brief Clean-up flag. If marked as rubbish, it will be deleted once the frame has finished.
         */
        bool m_IsRubbish;

        /**
         * @brief Generates a unique ID for each object.
         * @return A unique identifier.
         */
        static unsigned int GenID();

        friend class Engine;
        friend class BaseObject;
        friend class SceneSubsystem;
        friend class ReflectionSubsystem;
    };
}
