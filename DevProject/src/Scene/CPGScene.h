#pragma once
#include "Denix/Scene/Scene.h"
#include "CPG/JobSubsystemWidget.h"
#include "Denix/Thread/Thread.h"
#include "Util/ActorGridSpawner.h"
using namespace Denix;

class CPGScene : public Scene
{
public:
    CPGScene();
    void BeginScene() override;
    void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    void DebugUI(float _deltaTime) override;
    

    Ref<Counter> DebugCounter;

    ActorGridSpawner m_GridSpawner;
    Ref<JobSubsystemWidget> m_JobSubsystemWidget; 
};