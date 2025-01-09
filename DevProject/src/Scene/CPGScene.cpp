#include "CPGScene.h"

#include "imgui.h"

using namespace Denix;

void CPGScene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::Update(_deltaTime, _waitCounter);
}

void CPGScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    if (ImGui::Begin(GetName().c_str()))
    {
            ImGui::End();
    }
}
