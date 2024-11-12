#pragma once
#include "Denix/Resource/Asset.h"
#include "Denix/Scene/Scene.h"

using namespace Denix;

class DevScene : public Scene
{
public:
    DevScene() = default;
    ~DevScene() override = default;

    void BeginScene() override;
    void Update(float _deltaTime) override;
    void DebugUI(float _deltaTime) override;

    void SpawnGrid();
    int GridSize = 10;
};