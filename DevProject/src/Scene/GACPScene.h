#pragma once

#include "Denix/Scene/Scene.h"
#include "Util/ActorGridSpawner.h"

namespace Denix
{
    class JobSubsystemWidget;
}

using namespace Denix;

class GACPScene: public Scene
{
public:
    GACPScene() ;
    void BeginScene() override;
    void DebugUI(float _deltaTime) override;
    


    ActorGridSpawner m_GridSpawner;
    Ref<JobSubsystemWidget> m_JobSubsystemWidget;
};