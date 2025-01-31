/**
 * @file TimerSubsystem.h
 * @brief Declaration of the TimerSubsystem class, handling frame timing and FPS management.
 */

#pragma once

#include "Denix/Core/Subsystem.h"
#include "Denix/Core.h"
#include "Timer.h"

namespace Denix
{
    class Profile;

    /**
     * @class TimerSubsystem
     * @brief A subsystem for managing time-related functionalities in the engine.
     */
    class TimerSubsystem final : public Subsystem<TimerSubsystem>
    {
    public:
        /**
         * @brief Constructs the TimerSubsystem.
         */
        TimerSubsystem();

        /**
         * @brief Default destructor.
         */
        ~TimerSubsystem() override = default;

        TimerSubsystem(const TimerSubsystem& _other) = delete; /**< Copy constructor is deleted. */
        TimerSubsystem(TimerSubsystem&& _other) noexcept = delete; /**< Move constructor is deleted. */
        TimerSubsystem& operator=(const TimerSubsystem& _other) = delete; /**< Copy assignment is deleted. */
        TimerSubsystem& operator=(TimerSubsystem&& _other) noexcept = delete; /**< Move assignment is deleted. */

        /**
         * @brief Retrieves the total elapsed time since the program started.
         * @return The elapsed time in seconds.
         */
        static float GetProgramElaspedTime();

        /**
         * @brief Retrieves the current frames per second (FPS).
         * @return The current FPS value.
         */
        static int GetFPS();

        /**
         * @brief Retrieves the maximum FPS limit.
         * @return A reference to the maximum FPS limit.
         */
        static int& GetMaxFPS() { return s_Instance->m_MaxLimitFPS; }

        /**
         * @brief Sets the maximum FPS limit.
         * @param _maxFPS The maximum FPS value to set.
         */
        static void SetMaxFPS(int _maxFPS) { s_Instance->m_MaxLimitFPS = _maxFPS; }

        /**
         * @brief Retrieves the time taken to complete a frame in seconds.
         * @return The frame time in seconds.
         */
        static float GetFrameTime();

        /**
         * @brief Retrieves the time taken to complete a frame in milliseconds.
         * @return The frame time in milliseconds.
         */
        static float GetFrameTimeMs();

        /**
         * @brief Retrieves the average frame time in seconds.
         * @return The average frame time in seconds.
         */
        static float GetAverageFrameTime();

        /**
         * @brief Retrieves the average frame time in milliseconds.
         * @return The average frame time in milliseconds.
         */
        static float GetAverageFrameTimeMs();

        /**
         * @brief Retrieves the delta time (time elapsed between frames).
         * @return The delta time in seconds.
         */
        static float GetDeltaTime() { return s_Instance->m_DeltaTime; }

        /**
         * @brief Retrieves the game time speed factor.
         * @return A reference to the game time speed multiplier.
         */
        static float& GetGameTimeSpeed() { return s_Instance->m_GameTimeSpeed; }

    private:
        /**
         * @brief Initializes the timer subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the timer subsystem.
         */
        void Deinitialize() override;

        /**
         * @brief Marks the beginning of a frame.
         */
        void BeginFrame();

        /**
         * @brief Marks the end of a frame.
         */
        void EndFrame();

        int m_FramesPerSecond; /**< Stores the current FPS value. */

        float m_FrameTime; /**< Time taken for a frame to complete in milliseconds. */
        float m_DeltaTime; /**< Time elapsed between frames. */
        float m_GameTimeSpeed; /**< Speed factor affecting game time progression. */
        int m_MaxLimitFPS; /**< Maximum FPS limit. */

        Ref<Profile> m_EngineProfile; /**< Engine profiling reference. */
        std::vector<Ref<Timer>> m_Timers; /**< Collection of active timers. */

        friend class Engine; /**< Grants Engine class access to private members. */
        friend class ProfileSubsystem; /**< Grants ProfileSubsystem access to private members. */
    };
}
