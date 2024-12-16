#pragma once

#include <random>

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Denix::Math
{
    float RandF(float _min, float _max);

  inline float RandF(float _min, float _max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution dis(_min, _max);
    return dis(gen);
}
}
