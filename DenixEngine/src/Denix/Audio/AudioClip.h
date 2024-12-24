#pragma once

#include "AudioPrimitive.h"

#include "Denix/Scene/Object.h"

namespace Denix
{
    class Asset;
    struct AudioBuffer;

    
    
    class AudioClip: public Object
    {
    public:
        AudioClip(const Ref<Asset>& _audioClipAsset);
        ~AudioClip() override;

        uint32_t GetBuffer() const { return m_Buffer; }
        uint32_t GetWavLength() const { return m_WavLength; }
        
        Ref<Asset> m_AudioClpAsset;

        SDL_AudioSpec m_ClipSpec;

    private:
        uint32_t m_Buffer;
        uint32_t m_WavLength;

        friend class AudioSource;
        friend class AudioSubsystem;
    };
}
