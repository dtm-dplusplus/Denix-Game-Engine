#pragma once
#include "Denix/UI/Widget/Button.h"
#include "Denix/UI/Widget/Canvas.h"




class MainMenuCanvas: public Denix::Canvas
{
public:
    MainMenuCanvas() = default;
    ~MainMenuCanvas() override = default;
    
    void Update(float _deltaTime, const Denix::Ref<Counter>& _waitCounter) override;
    void BeginScene() override;
};


