/// @file AudioSubsystem.h
/// @brief Defines the AudioSubsystem class for handling audio functionality within the Denix engine.

#pragma once

#include "Denix/Core/Subsystem.h"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_events.h>

#include "al/al.h"
#include "al/alc.h"
#include "al/alext.h"
#include "AudioClip.h"
#include "AudioSource.h"

namespace Denix
{
    /**
     * @class AudioSubsystem
     * @brief Manages the audio system within the engine, handling audio playback and sources.
     */
    class AudioSubsystem : public Subsystem<AudioSubsystem>
    {
    public:
        /**
         * @brief Constructs the AudioSubsystem.
         */
        AudioSubsystem();

        /**
         * @brief Destroys the AudioSubsystem.
         */
        ~AudioSubsystem() override = default;

        /// Deleted copy constructor.
        AudioSubsystem(const AudioSubsystem& _other) = delete;
        /// Deleted move constructor.
        AudioSubsystem(AudioSubsystem&& _other) noexcept = delete;
        /// Deleted copy assignment operator.
        AudioSubsystem& operator=(const AudioSubsystem& _other) = delete;
        /// Deleted move assignment operator.
        AudioSubsystem& operator=(AudioSubsystem&& _other) noexcept = delete;

        /**
         * @brief Plays an audio clip as a single instance.
         * @param _audioClip The audio clip to be played.
         */
        static void PlayAudioClipSingle(const Ref<AudioClip>& _audioClip);

        /**
         * @brief Creates a new audio source.
         * @return A reference to the newly created AudioSource.
         */
        static Ref<AudioSource> CreateNewAudioSource();

    private:
        /**
         * @brief Processes an SDL audio event.
         * @param _event The SDL event related to audio.
         */
        void ProcessAudioEvent(const SDL_Event& _event);

        /**
         * @brief Initializes the audio subsystem.
         */
        void Initialize() override;

        /**
         * @brief Deinitializes the audio subsystem.
         */
        void Deinitialize() override;

        ALCdevice* m_Device;  ///< Pointer to the OpenAL device.
        ALCcontext* m_Context; ///< Pointer to the OpenAL context.

        Ref<AudioSource> m_GlobalAudioSource; ///< Global audio source for the engine.

        friend class Engine;
        friend class AudioSource;
    };
}
