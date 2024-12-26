#pragma once
#include "Denix/Asset/Asset.h"
#include "Denix/Scene/Scene.h"
#include "Util/ActorGridSpawner.h"

using namespace Denix;

class DevScene : public Scene
{
public:
    DevScene() = default;
    ~DevScene() override = default;

    void BeginScene() override;
    void Update(float _deltaTime) override;
    void DebugUI(float _deltaTime) override;

   ActorGridSpawner m_GridSpawner;
};