#pragma once

#include <random>

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Denix::Math
{
    inline int Rand(const int _min, const int _max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution dis(_min, _max);
        return dis(gen);
    }

    inline float RandF(const float _min, const float _max)
    {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution dis(_min, _max);
    return dis(gen);
    }
}
