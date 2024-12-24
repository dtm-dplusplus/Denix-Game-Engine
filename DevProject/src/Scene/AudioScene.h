#pragma once
#include "Denix/Scene/Scene.h"

namespace Denix
{
    class AudioSource;
}

namespace Denix
{
    class AudioClip;

    class AudioScene: public Scene
    {
    public:
        AudioScene() = default;
        void BeginScene() override;

        void DebugUI(float _deltaTime) override;
        Ref<AudioClip> Clip;
        Ref<AudioSource> Source;
        Ref<Asset> ClipAsset;
    };
}
