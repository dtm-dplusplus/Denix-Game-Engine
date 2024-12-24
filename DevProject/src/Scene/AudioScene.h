#pragma once
#include "Denix/Audio/AudioClip.h"
#include "Denix/Scene/Scene.h"

namespace Denix
{
    class AudioScene: public Scene
    {
    public:
        AudioScene() = default;
        void BeginScene() override;

        Ref<AudioClip> Clip;
    };
}
