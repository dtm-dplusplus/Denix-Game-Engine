#pragma once
#include <PxScene.h>

#include "Denix/Scene/Scene.h"


using namespace Denix;

class DevScene : public Scene
{
public:
    DevScene() = default;
    ~DevScene() override = default;

private:
    void BeginScene() override;
    void EndScene() override;

    void BeginPlay() override;
    void EndPlay() override;
    void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    void ToolUpdate(float _deltaTime, const Ref<Counter>& _waitCounter) override;
};