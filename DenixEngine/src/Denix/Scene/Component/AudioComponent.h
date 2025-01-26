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
        AudioComponent() : Component(ObjectInit("Audio Component"))
        {
            m_ClassName = "AudioComponent";
        }

        void Serialize(YAML::Emitter& _out) override;
        void Deserialize(const YAML::Node& _in) override;
        Ref<AudioClip> m_AudioClip;
    };
}
