#pragma once
#include "Denix/Scene/Scene.h"

class Character;
class GameOverCanvas;

class GEPScene: public Denix::Scene
{
public:
    GEPScene() = default;
    ~GEPScene() override = default;

    void BeginScene() override;
    void EndScene() override;
    void Update(float _deltaTime, const Denix::Ref<Counter>& _waitCounter) override;
    
    void DebugUI(float _deltaTime, const Denix::Ref<Counter>& _waitCounter) override;
    
    Denix::Ref<Character> m_Character;
    
    Denix::Ref<Denix::Camera> m_CharacterCamera;
    
    float ShootForce = 50.0f;
    
    int Health;
    
    bool m_GameOver;

    Denix::Ref<GameOverCanvas> m_GameOverCanvas;
};

