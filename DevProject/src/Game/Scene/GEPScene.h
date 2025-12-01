#pragma once
#include "Denix/Scene/Scene.h"

namespace Denix
{
    class AudioClip;
}

namespace Denix
{
    class AudioSource;
}

class GameOverCanvas;
class MainMenuCanvas;

class GEPScene: public Denix::Scene
{
public:
    GEPScene() = default;
    ~GEPScene() override = default;

    void BeginPlay() override;

    void EndScene() override;
    void Update(float _deltaTime, const Denix::Ref<Counter>& _waitCounter) override;
    
    void ToolUpdate(float _deltaTime, const Denix::Ref<Counter>& _waitCounter) override;
    
    float ShootForce = 50.0f;
    int CubeMultiplier = 2;
    
    Denix::Ref<MainMenuCanvas> m_MenuCanvas;
    
    Denix::Ref<Denix::AudioSource> m_MusicAudioSource;
    Denix::WRef<Denix::AudioClip> m_MusicClip;
};

