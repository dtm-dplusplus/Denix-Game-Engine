#pragma once

#include "Denix/Scene/Scene.h"

using namespace Denix;


class PhysicsScene: public Scene
{
public:
    PhysicsScene();
    PhysicsScene(const ObjectInit& _objInit);

    void BeginScene() override;
    void BeginPlay() override;
    void DebugUI(float _deltaTime) override;
    
    glm::vec3 StartImpulse = { 35.0f, 0.0f, 0.0f };
    std::vector<Ref<Sphere>> Spheres;
};
