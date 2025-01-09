#pragma once
#include "Denix/Scene/Scene.h"
#include "Denix/Thread/Thread.h"
#include "Util/ActorGridSpawner.h"


namespace Denix
{
    class JobSubsystemWidget;

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
}
