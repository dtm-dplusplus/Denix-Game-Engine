#pragma once
#include "Denix/Core.h"

namespace Denix
{
    class Scene;

    /**
     * \brief GACPSpawner struct for spawning grid objects in a scene.
     */
    struct GACPSpawner
    {
    public:
        /**
         * \brief Spawns a grid of objects in the given scene.
         * \param _scene Reference to the scene where the grid will be spawned.
         */
        void SpawnGrid(const Ref<Scene>& _scene) const;

        /**
         * \brief Size of the grid to spawn.
         */
        int GridSize = 10;

        /**
         * \brief Height at which the grid will be spawned.
         */
        inline static float SpawnHeight = 25.0f;
    };
}