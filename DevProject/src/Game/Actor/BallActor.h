#pragma once
#include "Denix/Scene/Actor.h"

class CubeActor: public Denix::Actor
{
    public:
     CubeActor();
};

class BallActor: public Denix::Actor
{
public:
    BallActor();
    void BeginScene() override;

    inline static float Mass = 100.0f;
};
