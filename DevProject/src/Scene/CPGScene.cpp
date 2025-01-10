#include "CPGScene.h"

#include "imgui.h"
#include "implot.h"
#include "Denix/Engine.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"
#include "Denix/Editor/Widget/Profile/JobTableWidget.h"
#include "Denix/Editor/Widget/Profile/JobSubsystemWidget.h"

using namespace Denix;

CPGScene::CPGScene()
{
    m_JobSubsystemWidget = MakeRef<JobSubsystemWidget>(ObjectInit("Job Subsystem Widget"));
}

void CPGScene::BeginScene()
{
    Scene::BeginScene();

    m_ActiveCamera->GetTransformComponent()->SetPosition(-0.0f, 100.0f, 280.0f);
    m_ActiveCamera->GetTransformComponent()->SetRotation({-30.0f, -70.0f, 0.0f});
}

void CPGScene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::Update(_deltaTime, _waitCounter);
}

void CPGScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::SetNextItemWidth(800);
    ImGui::Begin(GetName().c_str());

    if (ImGui::CollapsingHeader("Dev Stuff", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Batch Update Actors", &SceneSubsystem::Get()->m_BatchUpdateActors);
        ImGui::SeparatorText("Spawner");
        ImGui::DragFloat("Spawn Height", &ActorGridSpawner::SpawnHeight, 1.0f, 0.0f, 1000.0f);
        ImGui::DragInt("Grid Size", &m_GridSpawner.GridSize, 1.0f, 1, 100);
        ImGui::SameLine();
        if (ImGui::Button("Spawn Grid"))  m_GridSpawner.SpawnGrid(shared_from_this());
        if (ImGui::CollapsingHeader("Job Subsystem", ImGuiTreeNodeFlags_DefaultOpen))
        {
            m_JobSubsystemWidget->Update(_deltaTime);
        }
    }
        ImGui::End();
}
