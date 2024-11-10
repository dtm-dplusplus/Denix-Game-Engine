#pragma once
#include <vector>
#include <GLM/vec3.hpp>

#include "Denix/Core.h"

using namespace Denix;

class Ray;
class Sphere;

class RayTracer
{
public:
    RayTracer(){}

    std::vector<Ref<Sphere>> m_Objects;

    glm::vec3 TraceRay(const Ray& _ray, int _depth);
};
