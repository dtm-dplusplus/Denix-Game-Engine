#pragma once
#include "Denix/Scene/Scene.h"

inline static int CharSize = 50;


namespace Denix
{
    class UIScene: public Scene
    {
    public:
        UIScene() = default;

        void BeginScene() override;
        void Update(float _deltaTime) override;
        
    };
}
