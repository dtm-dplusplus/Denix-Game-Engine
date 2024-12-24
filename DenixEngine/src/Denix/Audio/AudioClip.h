#pragma once

#include "AudioPrimitive.h"

#include "Denix/Scene/Object.h"

namespace Denix
{
    class Asset;
    struct AudioBuffer;

    enum class AudioClipState: uint8_t
    {
        Stopped = 0,
        Playing = 1,
        Paused = 2
    };
    
    class AudioClip: public Object
    {
    public:
        AudioClip(const Ref<Asset>& _audioClipAsset);
        ~AudioClip() override;

        void Play() const;
        void Stop() const;
        void Pause() const;

        Ref<Asset> m_AudioClpAsset;
        
        uint32_t m_Buffer;
        uint32_t m_Source;

        SDL_AudioSpec m_ClipSpec;
        uint32_t m_WavLength;

        mutable AudioClipState m_State;
    };
}
