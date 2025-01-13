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
    void Update(float _deltaTime) override;
    void DebugUI(float _deltaTime) override;
    Ref<Character> m_Character;
    Ref<Camera> m_CharacterCamera;
};

