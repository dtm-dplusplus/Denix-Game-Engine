/// @file AudioSource.h
/// @brief Defines the AudioSource class for managing audio playback in the Denix engine.

#pragma once
#include <cstdint>
#include <GLM/vec3.hpp>

#include "Denix/Core/BaseObject.h"
#include "Denix/Core/Object.h"

namespace Denix
{
    class AudioClip;

    /**
     * @enum SourceState
     * @brief Represents the state of an audio source.
     */
    enum class SourceState : uint8_t
    {
        Stopped = 0, ///< The audio source is stopped.
        Playing = 1, ///< The audio source is playing.
        Paused = 2   ///< The audio source is paused.
    };

    /**
     * @class AudioSource
     * @brief Handles audio playback, position, and properties in the Denix engine.
     */
    class AudioSource : public BaseObject
    {
    public:
        /**
         * @brief Constructs an AudioSource.
         */
        AudioSource();

        /**
         * @brief Destroys the AudioSource.
         */
        ~AudioSource() override;

        /**
         * @brief Plays the associated audio clip.
         */
        void Play() const;

        /**
         * @brief Stops the currently playing audio.
         */
        void Stop() const;

        /**
         * @brief Pauses the currently playing audio.
         */
        void Pause() const;

        /**
         * @brief Gets the current state of the audio source.
         * @return The current SourceState.
         */
        SourceState GetState() const { return m_State; }

        /**
         * @brief Gets the maximum number of mono audio sources.
         * @return The maximum number of mono sources.
         */
        static int GetMaxMonoSources() { return s_MaxMonoSources; }

        /**
         * @brief Gets the maximum number of stereo audio sources.
         * @return The maximum number of stereo sources.
         */
        static int GetMaxStereoSources() { return s_MaxStereoSources; }

        /**
         * @brief Sets the audio clip to be played by this source.
         * @param _audioClip The audio clip reference.
         */
        void SetAudioClip(const Ref<AudioClip>& _audioClip);

        float m_Pitch = 1.0f; ///< Playback pitch modifier.
        float m_Gain = 1.0f; ///< Playback volume modifier.
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f }; ///< 3D position of the audio source.
        glm::vec3 m_Velocity = { 0.0f, 0.0f, 0.0f }; ///< Velocity of the audio source for Doppler effects.
        bool m_Looping = false; ///< Determines if the audio should loop.


    private:
        WRef<AudioClip> m_AudioClip; ///< Weak reference to the assigned audio clip.

        uint32_t m_Source; ///< Internal identifier for the audio source.

        mutable SourceState m_State = SourceState::Stopped; ///< Current state of the audio source.

        inline static uint32_t s_MonoSources = 0; ///< Number of active mono sources.
        inline static int s_MaxMonoSources = -1; ///< Maximum allowed mono sources.

        inline static uint32_t s_StereoSources = 0; ///< Number of active stereo sources.
        inline static int s_MaxStereoSources = -1; ///< Maximum allowed stereo sources.

        friend class AudioSubsystem;
    };
}