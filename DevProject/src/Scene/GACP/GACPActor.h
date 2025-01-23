#pragma once
#include "Denix/Scene/Actor/Shapes.h"

namespace Denix
{
    /**
     * Utility class for creating a GACP Actor
     */
    class GACPActor: public Cube
    {
    public:
        GACPActor();

        void Update(float _deltaTime) override;
        void RandomModel() const;
    };
}