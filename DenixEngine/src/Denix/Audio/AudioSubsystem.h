#pragma once

#include "Denix/Core/Subsystem.h"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_events.h>

#include "al/al.h"
#include "al/alc.h"
#include "al/alext.h"
#include "AudioClip.h"
#include "AudioSource.h"
#include "Denix/Scene/Component/AudioComponent.h"

namespace Denix
{
    class AudioSubsystem: public Subsystem<AudioSubsystem>
    {
    public:
        AudioSubsystem();

        ~AudioSubsystem() override = default;

        AudioSubsystem(const AudioSubsystem& _other) = delete;
        AudioSubsystem(AudioSubsystem&& _other) noexcept = delete;
        AudioSubsystem& operator=(const AudioSubsystem& _other) = delete;
        AudioSubsystem& operator=(AudioSubsystem&& _other) noexcept = delete;

    private:
        void ProcessAudioEvent(const SDL_Event& _event);

        void Initialize() override;
        void Deinitialize() override;
        ALCdevice* m_Device;
        ALCcontext* m_Context;

        std::vector<Ref<AudioSource>> m_AudioSources;

        
        friend class Engine;
        friend class AudioSource;
    };
}
