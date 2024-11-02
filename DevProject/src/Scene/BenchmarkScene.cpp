#include "BenchmarkScene.h"

#include "imgui.h"
#include "CPG/Ray.h"

using namespace Denix;

BenchmarkScene::BenchmarkScene(const Ref<Asset>& _sceneAsset): Scene(_sceneAsset)
{
}

void BenchmarkScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);
    
}

void BenchmarkScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    if (ImGui::Begin(m_SceneName.c_str()))
    {

        ImGui::End();
    }
}