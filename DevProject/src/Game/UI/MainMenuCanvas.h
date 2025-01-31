#pragma once
#include "Denix/UI/Widget/Canvas.h"


class MainMenuCanvas: public Denix::Canvas
{
public:
    MainMenuCanvas();
    ~MainMenuCanvas() override = default;
    
    void Update(float _deltaTime, const Denix::Ref<Counter>& _waitCounter) override;
    void BeginScene() override;
};


