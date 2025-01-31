/**
 * @file ReflectionSubsystem.h
 * @brief Declaration of the ReflectionSubsystem class, responsible for object reflection and dynamic instantiation.
 */

#pragma once

#include <functional>
#include <map>
#include "Denix/Core/BaseObject.h"
#include "Denix/Core/Subsystem.h"
#include "Denix/Core/Reflection/ReflectionHelper.h"

namespace Denix
{
    /**
     * @class ReflectionSubsystem
     * @brief Manages runtime reflection and object instantiation.
     */
    class ReflectionSubsystem : public Subsystem<ReflectionSubsystem>
    {
    public:
        /**
         * @brief Default constructor.
         */
        ReflectionSubsystem() = default;

        /**
         * @brief Default destructor.
         */
        ~ReflectionSubsystem() override = default;

        using CreateFunc = std::function<Ref<Object>()>;

        /**
         * @brief Registers a class for reflection.
         * @tparam T The class type to register.
         */
        template <typename T>
        static void Register()
        {
            const std::string className = ReflectionHelper::GetClassNameDE<T>();

            static_assert(IsBase<Object, T>(), "Class must be derived from Object");

            const CreateFunc _createFunc = [] { return MakeRef<T>(); };
            s_Instance->m_CreateFuncs[className] = _createFunc;
        }

        /**
         * @brief Creates an instance of a registered class.
         * @tparam T The expected return type (default is Object).
         * @param _className The name of the class to instantiate.
         * @return A reference to the newly created object, or nullptr if the class is not registered.
         */
        template <typename T = Object>
        static Ref<T> Create(const std::string& _className)
        {
            if (const auto it = s_Instance->m_CreateFuncs.find(_className); it != s_Instance->m_CreateFuncs.end())
            {
                if (Ref<Object> obj = it->second())
                {
                    obj->m_ClassName = _className;
                    return CastRef<T>(obj);
                }
            }
            return nullptr;
        }

        /**
         * @brief Retrieves an instance of a registered class.
         * @param _className The name of the class.
         * @return A reference to the object if found, otherwise nullptr.
         */
        static Ref<Object> GetType(const std::string& _className)
        {
            if (const auto it = s_Instance->m_CreateFuncs.find(_className); it != s_Instance->m_CreateFuncs.end())
            {
                return it->second();
            }
            return nullptr;
        }

        /**
         * @brief Checks if a class is registered.
         * @param _className The name of the class.
         * @return True if the class is registered, false otherwise.
         */
        static bool ClassExists(const std::string& _className)
        {
            return s_Instance->m_CreateFuncs.contains(_className);
        }

        /**
         * @brief Retrieves the map of registered class creation functions.
         * @return A reference to the map of class creation functions.
         */
        static std::map<std::string, CreateFunc>& GetCreateFuncs() { return s_Instance->m_CreateFuncs; }

    private:
        /**
         * @brief Initializes the reflection subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the reflection subsystem.
         */
        void Deinitialize() override;

        std::map<std::string, CreateFunc> m_CreateFuncs; /**< Map storing registered class creation functions. */

        friend class Engine; /**< Grants Engine class access to private members. */
    };
};
