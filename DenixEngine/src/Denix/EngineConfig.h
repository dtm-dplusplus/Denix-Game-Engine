#pragma once

namespace Denix
{
    /**
     * @brief Configuration settings for the Engine.
     */
    struct EngineConfig
    {
        //  Engine Configuration
        /**
         * @brief Name of the project.
         */
        std::string ProjectName;

        /**
         * @brief Path to the startup scene.
         */
        std::string StartupScenePath;

        // Input Configuration
        /**
         * @brief Enable or disable logging of mouse motion events.
         */
        bool MouseMotionLogging = false;

        /**
         * @brief Enable or disable logging of mouse button events.
         */
        bool MouseButtonLogging = false;

        /**
         * @brief Enable or disable logging of keyboard events.
         */
        bool KeyboardLogging = false;

        /**
         * @brief Enable or disable logging of gamepad events.
         */
        bool GamepadLogging = false;

        // Physics Configuration
        /**
         * @brief Enable or disable logging of physics events.
         */
        bool PhysicsLogging = false;
    };
}
