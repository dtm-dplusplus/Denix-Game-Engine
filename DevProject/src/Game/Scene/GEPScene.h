#pragma once
#include "imgui.h"
#include "Denix/Scene/Scene.h"

class Character;

using namespace Denix;

class GEPScene: public Scene
{
public:
    GEPScene();

    void BeginScene() override;
    void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    void DebugUI(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    Ref<Character> m_Character;
    Ref<Camera> m_CharacterCamera;
    float ShootForce = 100.0f;
};

