#pragma once
#include "Denix/Scene/Scene.h"
#include "Game/Button.h"

class MainMenuCanvas;
using namespace Denix;


class MainMenu: public Scene
{
public:
    void BeginScene() override;
    void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    Ref<MainMenuCanvas> m_Canvas;
};
