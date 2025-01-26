#pragma once
#include "Denix/Scene/Actor.h"
#include "Denix/Scene/Actor/Shapes.h"

class GEPActor: public  Denix::Cube
{
public:
     GEPActor();

     void OnCollisionEnter(const Denix::Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point) override;
     void OnCollisionExit(const Denix::Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point)override;
};
