/**
 * @file EventSubsystem.h
 * @brief Declaration of the EventSubsystem class, responsible for handling SDL events in the engine.
 */

#pragma once

#include "Denix/Core/Subsystem.h"
#include <SDL3/SDL_events.h>

namespace Denix
{
    class SDL_GLWindow;
    class InputSubsystem;
    class WindowSubsystem;

    /**
     * @class EventSubsystem
     * @brief Handles and processes various SDL events for the engine.
     */
    class EventSubsystem : public Subsystem<EventSubsystem>
    {
    public:
        /**
         * @brief Default constructor.
         */
        EventSubsystem() = default;

        /**
         * @brief Default destructor.
         */
        ~EventSubsystem() override = default;

        // Delete copy and move constructors
        EventSubsystem(const EventSubsystem& _other) = delete;
        EventSubsystem(EventSubsystem&& _other) noexcept = delete;
        EventSubsystem& operator=(const EventSubsystem& _other) = delete;
        EventSubsystem& operator=(EventSubsystem&& _other) noexcept = delete;

    private:
        /**
         * @brief Processes application-specific SDL events.
         * @param _event The SDL event to process.
         */
        void ProcessApplicationEvent(const SDL_Event& _event);

        /**
         * @brief Processes display-related SDL events.
         * @param _event The SDL event to process.
         */
        void ProcessDisplayEvent(const SDL_Event& _event);

        /**
         * @brief Processes window-related SDL events.
         * @param _event The SDL event to process.
         */
        void ProcessWindowEvent(const SDL_Event& _event);

        /**
         * @brief Processes file-related SDL events.
         * @param _event The SDL event to process.
         */
        void ProcessFileEvent(const SDL_Event& _event);

        /**
         * @brief Processes audio-related SDL events.
         * @param _event The SDL event to process.
         */
        void ProcessAudioEvent(const SDL_Event& _event);

        /**
         * @brief Processes rendering-related SDL events.
         * @param _event The SDL event to process.
         */
        void ProcessRenderEvent(const SDL_Event& _event);

        /**
         * @brief Initializes the event subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the event subsystem.
         */
        void Deinitialize() override;

        /**
         * @brief Updates the event subsystem.
         * @param _deltaTime The time elapsed since the last update.
         * @param _waitCounter Reference counter for synchronization.
         */
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        WRef<SDL_GLWindow> m_WindowRef; /**< Weak reference to the SDL window. */
        WRef<InputSubsystem> m_InputRef; /**< Weak reference to the input subsystem. */
        WRef<WindowSubsystem> m_WindowSubsystemRef; /**< Weak reference to the window subsystem. */

        friend class Engine; /**< Grants Engine class access to private members. */
    };
}
