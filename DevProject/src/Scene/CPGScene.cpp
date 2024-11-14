#include "CPGScene.h"

#include "imgui.h"

using namespace Denix;

void CPGScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);
}

void CPGScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    if (ImGui::Begin(GetName().c_str()))
    {
            ImGui::End();
    }
}
