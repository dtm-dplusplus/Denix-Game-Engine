#pragma once
#include "Denix/Scene/Scene.h"

namespace Denix
{
    class AudioClip;
    class AudioSource;

    class AudioScene: public Scene
    {
    public:
        AudioScene() = default;
        void BeginScene() override;
        void EndScene() override;

        void DebugUI(float _deltaTime) override;
        WRef<AudioClip> Clip;
        Ref<AudioSource> Source;
    };
}
