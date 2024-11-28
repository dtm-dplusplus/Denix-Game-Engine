#pragma once

#include "Denix/Core/Timer.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Thread/Thread.h"
#include "Util/ActorGridSpawner.h"

using namespace Denix;
struct ActorGridSpawner;

class ThreadScene: public Scene
{
public:
    ~ThreadScene() override;
    void BeginScene() override;
    void Update(float _deltaTime) override;
    void DebugUI(float _deltaTime) override;

    
    void JobA();
    void JobAArg(int _arg);
    void JobB();
    void JobC();
    

    Ref<Counter> DebugCounter;
    Ref<bool> RenderParallel;

    ActorGridSpawner m_GridSpawner;
};