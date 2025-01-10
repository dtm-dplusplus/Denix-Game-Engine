#pragma once

#include "Denix/Scene/Scene.h"
#include "GACPSpawner.h"

namespace Denix
{
    class JobSubsystemWidget;

    class GACPScene final: public Scene
    {
    public:
        GACPScene();
        void BeginScene() override;
        void DebugUI(float _deltaTime) override;
    
        GACPSpawner m_GridSpawner;
        Ref<JobSubsystemWidget> m_JobSubsystemWidget;
    };
}
