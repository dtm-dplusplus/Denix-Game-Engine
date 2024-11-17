#pragma once

#include "Denix/Core/Timer.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Thread/Thread.h"

using namespace Denix;

class ThreadScene: public Scene
{
public:
    ThreadScene();
    ~ThreadScene();
    void DebugUI(float _deltaTime) override;
    void Work();
    void InfiniteWork(bool& _ShouldWork);

    Ref<Timer>  WorkTimer;
    bool WorkArg = false;
    Ref<Thread> DebugThread; 
};