#pragma once

#include "Denix/Core/Timer.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Thread/Thread.h"

using namespace Denix;

class ThreadScene: public Scene
{
public:
    ~ThreadScene() override;
    void BeginScene() override;
    void DebugUI(float _deltaTime) override;

    
    void JobA();
    void JobB();
    void JobC();
    

    Ref<Thread> DebugThread; 
};