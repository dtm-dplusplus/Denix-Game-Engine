/**
 * @file InputSubsystem.h
 * @brief Declaration of the InputSubsystem class, responsible for handling input events.
 */

#pragma once

#include "Denix/Core/Subsystem.h"
#include <SDL3/SDL_events.h>
#include "Denix/Core.h"
#include "InputPrimitive.h"
#include "InputHelper.h"

namespace Denix
{
    class SDL_GLWindow;

    /**
     * @class InputSubsystem
     * @brief Manages input events including keyboard and mouse input.
     */
    class InputSubsystem : public Subsystem<InputSubsystem>
    {
    public:
        /**
         * @brief Default constructor.
         */
        InputSubsystem() = default;

        /**
         * @brief Default destructor.
         */
        ~InputSubsystem() override = default;

        InputSubsystem(const InputSubsystem& _other) = delete;
        InputSubsystem(InputSubsystem&& _other) noexcept = delete;
        InputSubsystem& operator=(const InputSubsystem& _other) = delete;
        InputSubsystem& operator=(InputSubsystem&& _other) noexcept = delete;

        /**
         * @brief Checks if a key is currently pressed.
         * @param _key Key code to check.
         * @return True if the key is down, false otherwise.
         */
        static bool IsKeyDown(KeyCode _key);

        /**
         * @brief Checks if a key is currently released.
         * @param _key Key code to check.
         * @return True if the key is up, false otherwise.
         */
        static bool IsKeyUp(KeyCode _key);

        /**
         * @brief Checks if a mouse button is currently pressed.
         * @param _button Mouse button index.
         * @return True if the button is down, false otherwise.
         */
        static bool IsMouseButtonDown(const int _button)
        {
            return s_Instance->m_Mouse->SDL_State & SDL_BUTTON(_button);
        }

        /**
         * @brief Retrieves the current mouse data.
         * @return Reference to the MouseData structure.
         */
        static MouseData& GetMouseData() { return s_Instance->m_Mouse->m_MouseData; }

        /**
         * @brief Retrieves the available input devices.
         */
        static void GetDevices();

        bool m_MouseLogging; /**< Flag for enabling mouse logging. */

    private:
        /**
         * @brief Processes generic input events.
         * @param _event SDL event reference.
         */
        void ProcessInputEvent(const SDL_Event& _event);

        /**
         * @brief Processes pen input events.
         * @param _event SDL event reference.
         */
        void ProcessPenEvent(const SDL_Event& _event);

        /**
         * @brief Processes camera-related input events.
         * @param _event SDL event reference.
         */
        void ProcessCameraEvent(const SDL_Event& _event);

        /**
         * @brief Initializes the input subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the input subsystem.
         */
        void Deinitialize() override;

        WRef<SDL_GLWindow> m_WindowRef; /**< Weak reference to the SDL window. */
        Ref<Keyboard> m_Keyboard; /**< Reference to the keyboard input handler. */
        Ref<Mouse> m_Mouse; /**< Reference to the mouse input handler. */

        friend class Engine;
        friend class EventSubsystem;
    };
}
