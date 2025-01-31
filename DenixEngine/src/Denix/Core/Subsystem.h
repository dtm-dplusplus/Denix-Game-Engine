/**
 * @file Subsystem.h
 * @brief Declaration of the SubsystemBase and Subsystem template class, responsible for managing subsystems in the engine.
 */

#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Thread/Counter.h"

namespace Denix
{
    /**
     * @class SubsystemBase
     * @brief Base class for all engine subsystems.
     */
    class SubsystemBase
    {
    public:
        /**
         * @brief Default constructor.
         */
        SubsystemBase() = default;

        /**
         * @brief Default virtual destructor.
         */
        virtual ~SubsystemBase() = default;

        // Delete copy and move constructors
        SubsystemBase(const SubsystemBase& _other) = delete;
        SubsystemBase(SubsystemBase&& _other) noexcept = delete;
        SubsystemBase& operator=(const SubsystemBase& _other) = delete;
        SubsystemBase& operator=(SubsystemBase&& _other) noexcept = delete;

        /**
         * @brief Initializes the subsystem.
         */
        virtual void Initialize() = 0;

        /**
         * @brief Deinitializes the subsystem.
         */
        virtual void Deinitialize() = 0;

        /**
         * @brief Updates the subsystem.
         * @param _deltaTime Time elapsed since the last update.
         * @param _waitCounter Synchronization counter.
         */
        virtual void Update(float _deltaTime, const Ref<Counter>& _waitCounter) {}

        /**
         * @brief Pre-update phase for the subsystem.
         * @param _deltaTime Time elapsed since the last update.
         * @param _waitCounter Synchronization counter.
         */
        virtual void PreUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) {}

        /**
         * @brief Post-update phase for the subsystem.
         * @param _deltaTime Time elapsed since the last update.
         * @param _waitCounter Synchronization counter.
         */
        virtual void PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) {}

        /**
         * @brief Checks if the subsystem is enabled.
         * @return True if enabled, false otherwise.
         */
        virtual bool IsEnabled() const { return m_Enabled; }

        /**
         * @brief Retrieves a reference to the enabled state.
         * @return A reference to the enabled flag.
         */
        virtual bool& IsEnabled() { return m_Enabled; }

        /**
         * @brief Sets the enabled state of the subsystem.
         * @param _enabled The new enabled state.
         */
        virtual void SetEnabled(const bool _enabled) { m_Enabled = _enabled; }

    protected:
        bool m_Enabled = true; /**< Flag indicating whether the subsystem is enabled. */

        friend class Engine; /**< Grants Engine class access to private members. */
    };

    /**
     * @class Subsystem
     * @brief Template class for defining specific engine subsystems.
     * @tparam T The derived subsystem type.
     */
    template <class T>
    class Subsystem : public SubsystemBase, public std::enable_shared_from_this<T>
    {
    public:
        /**
         * @brief Default constructor.
         */
        Subsystem() = default;

        /**
         * @brief Default virtual destructor.
         */
        ~Subsystem() override = default;

        // Delete copy and move constructors
        Subsystem(const Subsystem& _other) = delete;
        Subsystem(Subsystem&& _other) noexcept = delete;
        Subsystem& operator=(const Subsystem& _other) = delete;
        Subsystem& operator=(Subsystem&& _other) noexcept = delete;

        /**
         * @brief Initializes the subsystem instance.
         */
        void Initialize() override
        {
            s_Instance = this->shared_from_this();
        }

        /**
         * @brief Deinitializes the subsystem instance.
         */
        void Deinitialize() override
        {
            s_Instance.reset();
        }

        /**
         * @brief Updates the subsystem.
         * @param _deltaTime Time elapsed since the last update.
         * @param _waitCounter Synchronization counter.
         */
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override {}

        /**
         * @brief Pre-update phase for the subsystem.
         * @param _deltaTime Time elapsed since the last update.
         * @param _waitCounter Synchronization counter.
         */
        void PreUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) override {}

        /**
         * @brief Post-update phase for the subsystem.
         * @param _deltaTime Time elapsed since the last update.
         * @param _waitCounter Synchronization counter.
         */
        void PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) override {}

        /**
         * @brief Checks if the subsystem is enabled.
         * @return True if enabled, false otherwise.
         */
        bool IsEnabled() const override { return m_Enabled; }

        /**
         * @brief Retrieves a reference to the enabled state.
         * @return A reference to the enabled flag.
         */
        bool& IsEnabled() override { return m_Enabled; }

        /**
         * @brief Sets the enabled state of the subsystem.
         * @param _enabled The new enabled state.
         */
        void SetEnabled(const bool _enabled) override { m_Enabled = _enabled; }

        /**
         * @brief Retrieves the instance of the subsystem.
         * @return A reference to the shared instance.
         */
        static Ref<T> GetInstance() { return s_Instance; }

    protected:
        bool m_Enabled = true; /**< Flag indicating whether the subsystem is enabled. */

        inline static Ref<T> s_Instance; /**< Shared instance of the subsystem. */

        friend class Engine; /**< Grants Engine class access to private members. */
    };
}
