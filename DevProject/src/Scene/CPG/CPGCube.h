#pragma once
#include "FrameGraphWidget.h"
#include "Denix/Scene/Object/Shapes/Shapes.h"

using namespace Denix;

class CPGCube: public Cube
{
public:
    CPGCube();

    void BeginPlay() override;
    void Update(float _deltaTime) override;
};
