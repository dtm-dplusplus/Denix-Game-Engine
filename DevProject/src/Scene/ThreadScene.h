#pragma once

#include "CPG/JobSubsystemWidget.h"
#include "Denix/Core/Timer.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Thread/Thread.h"
#include "Util/ActorGridSpawner.h"

using namespace Denix;
struct ActorGridSpawner;

class ThreadScene: public Scene
{
public:
    ThreadScene() ;
    void BeginScene() override;
    void Update(float _deltaTime) override;
    void DebugUI(float _deltaTime) override;
    

    Ref<Counter> DebugCounter;
    Ref<bool> RenderParallel;

    ActorGridSpawner m_GridSpawner;
    Ref<JobSubsystemWidget> m_JobSubsystemWidget;
};