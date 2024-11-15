#pragma once

#include "Denix/Core/Timer.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Thread/Thread.h"

using namespace Denix;

class ThreadScene: public Scene
{
public:
    ThreadScene();
    
    void BeginScene() override;

    void DebugUI(float _deltaTime) override;
    void Work(int _arg);
    void InfiniteWork(bool& _ShouldWork);

    Ref<Timer>  WorkTimer;
    bool WorkArg = false;
    std::vector<Ref<Thread>> Threads; 
};

