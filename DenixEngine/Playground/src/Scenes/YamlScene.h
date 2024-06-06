#pragma once
#include "Denix\Scene\Scene.h"

namespace Denix
{
    
    class YamlScene :public Scene
    {
    public:
        YamlScene();

        bool Load() override;
        void Update(float _deltaTime) override;

    };
}


