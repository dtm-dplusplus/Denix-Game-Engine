#pragma once
#include <al/al.h>

#include "Denix/Scene/Object.h"

namespace Denix
{
    struct AudioBuffer
    {
        void GenBuffer();
        void BufferData( ALenum format, const ALvoid* data, ALsizei size, ALsizei samplerate );
        uint32_t m_Buffer;
    };

    struct AudioSource
    {
        void GenSource();
        uint32_t m_Source;
    };
    
    class AudioClip: public Object
    {
    public:
        AudioClip();
        AudioClip(const ObjectInit& _objInit);
        ~AudioClip() override;

        void Play() const;
        void Stop() const;

        AudioBuffer m_Buffer;
        AudioSource m_Source;
    };
}
