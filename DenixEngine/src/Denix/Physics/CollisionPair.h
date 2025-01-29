#pragma once
#include <glm/vec3.hpp>

#include "Denix/Core.h"

namespace Denix
{
    class PhysicsComponent;

    struct CollisionPair
    {
        glm::vec3 Point;
        glm::vec3 Normal;
        glm::vec3 Impulse;

    private:
        std::array<PhysicsComponent*, 2> m_Actors;
        bool CollisionEnter;

        friend class CollisionCallback;
        friend class PhysicsComponent;
        friend class Actor;
    };
}
