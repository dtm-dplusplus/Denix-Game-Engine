/**
 * @file WindowSubsystem.h
 * @brief Declaration of the WindowSubsystem class, responsible for window management operations.
 */

#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Subsystem.h"
#include "Denix/Video/Window/Window.h"
#include "GL/Viewport.h"

namespace Denix
{
    /**
     * @class WindowSubsystem
     * @brief Manages window-related operations such as resizing, fullscreen toggling, and rendering.
     */
    class WindowSubsystem : public Subsystem<WindowSubsystem>
    {
    public:
        /**
         * @brief Default constructor.
         */
        WindowSubsystem() = default;

        /**
         * @brief Default destructor.
         */
        ~WindowSubsystem() override = default;

        /**
         * @brief Toggles between fullscreen and windowed mode.
         */
        static void ToggleFullscreen();

        /**
         * @brief Retrieves the main application window.
         * @return A reference to the SDL_GLWindow instance.
         */
        static Ref<SDL_GLWindow> GetWindow() { return s_Instance->m_Window; }

        /**
         * @brief Gets the current window size.
         * @return The window size as a glm::vec2.
         */
        static glm::vec2 GetWindowSize() { return s_Instance->m_Window->GetWindowSize(); }

        /**
         * @brief Gets the current window width.
         * @return The window width in pixels.
         */
        static int GetWindowWidth() { return s_Instance->m_Window->GetWidth(); }

        /**
         * @brief Gets the current window height.
         * @return The window height in pixels.
         */
        static int GetWindowHeight() { return s_Instance->m_Window->GetHeight(); }

    private:
        /**
         * @brief Prepares the new frame for rendering.
         */
        void NewFrame();

        /**
         * @brief Presents the rendered frame to the screen.
         */
        void PresentFrame();

        /**
         * @brief Initializes the window subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the window subsystem.
         */
        void Deinitialize() override;

        Ref<SDL_GLWindow> m_Window; /**< Reference to the main window. */

        friend class Engine;
    };
}
