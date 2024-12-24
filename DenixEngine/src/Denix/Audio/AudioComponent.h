#pragma once
#include "Denix/Scene/Component.h"

namespace Denix
{
    class AudioClip;
}

namespace Denix
{
    class AudioComponent: public Component
    {
    public:
        AudioComponent() : Component(ObjectInit("Audio Component")) {}

        Ref<AudioClip> m_AudioClip;
    };
}
