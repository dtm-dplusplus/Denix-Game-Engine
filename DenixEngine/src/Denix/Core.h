/**
 * @file Core.h
 * @brief Defines smart pointer utilities and assertion macros for the Denix engine.
 *
 * Provides typedefs and helper functions for shared, unique, and weak pointers.
 */

#pragma once

#include <memory>
#include "Denix/Core/Log/Logger.h"

namespace Denix
{

    // References for further reading:
    // https://stackoverflow.com/questions/5252375/custom-c-assert-macro
    // https://en.cppreference.com/w/cpp/memory/weak_ptr
    // https://en.cppreference.com/w/cpp/memory/shared_ptr
    // https://en.cppreference.com/w/cpp/memory/unique_ptr
    // https://en.cppreference.com/w/cpp/language/dynamic_cast
    // https://en.cppreference.com/w/cpp/types/is_base_of

    /**
     * @brief Custom assertion macro.
     *
     * Logs a critical error message and aborts execution if the assertion fails.
     *
     * @param x Condition to assert.
     * @param msg Message to log if assertion fails.
     */
#define DE_ASSERT(x, msg) \
	do { \
	if (!(x)) { \
	DE_LOG(LogCore, Critical, "Assertion Failed: {0}", msg); \
	std::abort(); \
	} \
	} while (0);

     /**
      * @brief Compile-time assertion macro.
      *
      * Generates a compiler error if the assertion fails.
      *
      * @param x Condition to assert.
      * @param msg Message to display if assertion fails.
      */
#define DE_STATIC_ASSERT(x,msg) static_assert(x, msg);

      // Smart Pointers

      /**
       * @brief Alias for std::shared_ptr.
       * @tparam T Type of the managed object.
       */
    template <typename T>
    using Ref = std::shared_ptr<T>;

    /**
     * @brief Creates a shared pointer.
     * @tparam T Type of the object.
     * @tparam Args Argument types for the constructor.
     * @param args Arguments for the constructor.
     * @return A shared pointer to the newly created object.
     */
    template <typename T, typename... Args>
    constexpr Ref<T> MakeRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    /**
     * @brief Alias for std::unique_ptr.
     * @tparam T Type of the managed object.
     */
    template <typename T>
    using URef = std::unique_ptr<T>;

    /**
     * @brief Creates a unique pointer.
     * @tparam T Type of the object.
     * @tparam Args Argument types for the constructor.
     * @param args Arguments for the constructor.
     * @return A unique pointer to the newly created object.
     */
    template <typename T, typename... Args>
    constexpr URef<T> MakeURef(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    /**
     * @brief Alias for std::weak_ptr.
     * @tparam T Type of the managed object.
     */
    template <typename T>
    using WRef = std::weak_ptr<T>;

    /**
     * @brief Casts a smart pointer to another type using dynamic_cast.
     * @tparam T Target type.
     * @tparam Arg Source pointer type.
     * @param arg Source pointer to cast.
     * @return A shared pointer of the target type, or nullptr if the cast fails.
     */
    template <typename T, typename Arg>
    constexpr Ref<T> CastRef(Arg&& arg)
    {
        auto castRef = std::dynamic_pointer_cast<T>(std::forward<Arg>(arg));
        return castRef ? castRef : nullptr;
    }

    // Pointer validation

    /**
     * @brief Checks if a shared pointer is valid.
     * @tparam T Type of the object.
     * @param _ref Shared pointer to check.
     * @return True if the pointer is valid, false otherwise.
     */
    template <typename T>
    constexpr bool IsValid(const Ref<T>& _ref)
    {
        return _ref.get() != nullptr;
    }

    /**
     * @brief Checks if a type is derived from a base class.
     * @tparam Base Base class.
     * @tparam Derived Derived class.
     * @return True if Derived is a subclass of Base, false otherwise.
     */
    template <typename Base, typename Derived>
    constexpr bool IsBase()
    {
        return std::is_base_of_v<Base, Derived>;
    }
}