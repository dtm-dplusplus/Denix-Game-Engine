#pragma once
#include "Denix/Core.h"


namespace Denix
{
    class Scene;
    
    struct GACPSpawner
    {
    public:
        void SpawnGrid(const Ref<Scene>& _scene) const;

        int GridSize = 10;
        inline static float SpawnHeight = 25.0f;
    };
}
