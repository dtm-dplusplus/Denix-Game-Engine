#pragma once

#include "Denix/Scene/Object/Shapes/Shapes.h"

using namespace Denix;

class Ray: public Cube
{
public:
    Ray();

    //void OnCollision(Ref<Actor>& _other, CollisionData& _collision) override;
    void EndPlay() override;
    void InitRay(const glm::vec3& _origin  = glm::vec3(0.0f), const glm::vec3& _direction = glm::vec3(0.0f));
    glm::vec3 m_Origin;
    glm::vec3 m_Direction;

    bool m_Collision = false;

    static float m_MoveSpeed;
    glm::vec3 m_MoveDir = glm::vec3(0.0f);
};
