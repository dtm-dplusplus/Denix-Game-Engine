#pragma once
#include "Denix/Scene/Actor.h"

class CubeActor: public Denix::Actor
{
    public:
     CubeActor();
};

class BallActor: public Denix::Actor
{
public:
    BallActor();
    void BeginScene() override;
    void OnCollisionEnter(const Denix::Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point);
    inline static float Mass = 100.0f;
    bool m_IsDead = false;
};
