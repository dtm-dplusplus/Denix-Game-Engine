#pragma once
#include "Denix/UI/Widget/Canvas.h"

class GameOverCanvas: public Denix::Canvas
{
public:
    GameOverCanvas() = default;
    ~GameOverCanvas() override = default;

    void BeginScene() override;

    void Update(float _deltaTime, const Denix::Ref<Counter>& _waitCounter) override;
};
