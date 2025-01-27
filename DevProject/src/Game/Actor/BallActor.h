#pragma once
#include "Denix/Scene/Actor.h"

class BallActor: public Denix::Actor
{
public:
    BallActor();
    void BeginScene() override;

    inline static float Mass = 100.0f;
};
