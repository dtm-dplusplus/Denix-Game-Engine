#pragma once
#include "Denix/Scene/Scene.h"

using namespace Denix;

class BenchmarkScene : public Scene
{
public:
    BenchmarkScene() = default;
    BenchmarkScene(const Ref<Asset>& _sceneAsset);
    ~BenchmarkScene() override = default;

    void Update(float _deltaTime) override;
    void DebugUI(float _deltaTime) override;
    
    Ref<class Ray> m_Ray;
 
};