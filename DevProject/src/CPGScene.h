#pragma once
#include "Denix/Scene/Scene.h"

namespace Denix
{
    class Timer;
}

using namespace Denix;

class CPGScene : public Scene
{
public:
    CPGScene() = default;
    CPGScene(const Ref<Asset>& _sceneAsset);
    ~CPGScene() override = default;

    void Update(float _deltaTime) override;
    bool ShowEngineContent = false;

    Ref<class Ray> m_Ray;
    Ref<Timer> SceneTimer;

    std::chrono::time_point<std::chrono::system_clock> start, end;
};