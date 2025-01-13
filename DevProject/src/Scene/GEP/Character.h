#pragma once
#include "Denix/Scene/Actor.h"

namespace Denix
{
    class Camera;
}

using namespace Denix;

class Character: public Actor
{
public:
    Character();

    void BeginPlay() override;
    void Update(float _deltaTime) override;

    bool IsOnGround();
    Ref<Camera> m_Camera;
    float m_JumpForce;
    float m_MoveSpeed;
};