#include "ThreadScene.h"

#include "imgui.h"
#include "implot.h"
#include "Denix/Engine.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"
#include "CPG/JobSubsystemWidget.h"
#include "Denix/Resource/ResourceSubsystem.h"

using namespace Denix;

ThreadScene::ThreadScene()
{
    m_JobSubsystemWidget = MakeRef<JobSubsystemWidget>(ObjectInit("Job Subsystem Widget"));
}

void ThreadScene::BeginScene()
{
    Scene::BeginScene();
}

void ThreadScene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::Update(_deltaTime, _waitCounter);
}

void ThreadScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::SetNextItemWidth(500);
    ImGui::Begin(GetName().c_str());

    if (ImGui::CollapsingHeader("Dev Stuff", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Batch Update Actors", &SceneSubsystem::Get()->m_BatchUpdateActors);
        ImGui::SeparatorText("Spawner");
        ImGui::DragFloat("Spawn Height", &ActorGridSpawner::SpawnHeight, 1.0f, 0.0f, 1000.0f);
        ImGui::DragInt("Grid Size", &m_GridSpawner.GridSize, 1.0f, 1, 100);
        ImGui::SameLine();
        if (ImGui::Button("Spawn Grid"))  m_GridSpawner.SpawnGrid(shared_from_this());
    }

    if (ImGui::CollapsingHeader("Job Subsystem", ImGuiTreeNodeFlags_DefaultOpen))
    {
        m_JobSubsystemWidget->Update(_deltaTime);
    }

        ImGui::End();
}