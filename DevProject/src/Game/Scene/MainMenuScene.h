#pragma once
#include "Denix/Scene/Scene.h"

class MainMenuCanvas;

class MainMenuScene: public Denix::Scene
{
public:
    void BeginScene() override;
    void EndScene() override;
    void DebugUI(float _deltaTime, const Denix::Ref<Counter>& _waitCounter) override;
    void Update(float _deltaTime, const Denix::Ref<Counter>& _waitCounter) override;
    Denix::Ref<MainMenuCanvas> m_MainMenuCanvas;
};
