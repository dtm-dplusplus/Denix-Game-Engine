#pragma once
#include "Denix/Scene/Scene.h"

class MainMenuCanvas;
using namespace Denix;


class MainMenuScene: public Scene
{
public:
    void BeginScene() override;
    void DebugUI(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    Ref<MainMenuCanvas> m_Canvas;
};
