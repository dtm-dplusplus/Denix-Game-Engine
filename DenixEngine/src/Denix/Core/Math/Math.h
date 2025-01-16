#pragma once

#include <random>

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Denix::Math
{
    inline glm::vec3 Degrees(const glm::vec3& _rads)
    {
        return {glm::degrees(_rads.x), glm::degrees(_rads.y), glm::degrees(_rads.z)};
    }

    inline glm::vec3 Degrees(const float _x, const float _y, const float _z)
    {
        return {glm::degrees(_x), glm::degrees(_y), glm::degrees(_z)};
    }
    
    inline float Degrees(const float _rad)
    {
        return glm::degrees(_rad);
    }
    
    inline glm::vec3 Radians(const glm::vec3& _degrees)
    {
        return {glm::radians(_degrees.x), glm::radians(_degrees.y), glm::radians(_degrees.z)};
    }

    inline glm::vec3 Radians(const float _x, const float _y, const float _z)
    {
        return {glm::radians(_x), glm::radians(_y), glm::radians(_z)};
    }

    inline float Radians(const float _deg)
    {
        return glm::radians(_deg);
    }
    
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
