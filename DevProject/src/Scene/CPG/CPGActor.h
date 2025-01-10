#pragma once
#include "Denix/Scene/Object/Shapes/Shapes.h"

using namespace Denix;

class CPGActor: public Cube
{
public:
    CPGActor();

    void BeginPlay() override;
    void Update(float _deltaTime) override;
    void RandomModel();
    inline static bool ChangeModel = true;
    inline static bool AsyncModelChange = true;
};
