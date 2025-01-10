#pragma once
#include "Denix/Scene/Object/Shapes/Shapes.h"

namespace Denix
{
    class GACPActor: public Cube
    {
    public:
        GACPActor();

        void Update(float _deltaTime) override;
        void RandomModel() const;
    };
}