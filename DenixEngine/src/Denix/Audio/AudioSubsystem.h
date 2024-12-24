#pragma once

#include "Denix/System/Subsystem.h"

#include <SDL3/SDL_audio.h>
#include "al/al.h"
#include "al/alc.h"
#include "al/alext.h"

namespace Denix
{
    struct AudioData
    {
        SDL_AudioSpec WavSpec;
        Uint32 WavLength;
        Uint8* WavBuffer;
    };
    
    class AudioSubsystem: public Subsystem
    {
    public:
        AudioSubsystem()
        {
            DE_LOG_CREATE(LogAudio)
            s_AudioSubsystem = this;
        }

        AudioSubsystem* Get() { return s_AudioSubsystem; }

    private:
        void Initialize() override;
        void Deinitialize() override;
        inline static AudioSubsystem* s_AudioSubsystem;
        ALCdevice* m_Device;
        ALCcontext* m_Context;

        friend class Engine;
    };
}
