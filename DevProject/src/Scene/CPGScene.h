#pragma once
#include "Denix/Scene/Scene.h"

using namespace Denix;

class CPGScene : public Scene
{
public:
    CPGScene() = default;
    CPGScene(const Ref<Asset>& _sceneAsset);
    ~CPGScene() override = default;

    void Update(float _deltaTime) override;
    void DebugUI(float _deltaTime) override;
    
    Ref<class Ray> m_Ray;
 
};