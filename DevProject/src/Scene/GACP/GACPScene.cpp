#include "GACPScene.h"

#include "imgui.h"
#include "Denix/Engine.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"
#include "Denix/Editor/Widget/Profile/JobSubsystemWidget.h"

using namespace Denix;

GACPScene::GACPScene()
{
    m_JobSubsystemWidget = MakeRef<JobSubsystemWidget>(ObjectInit("Job Subsystem Widget"));
}

void GACPScene::BeginScene()
{
    Scene::BeginScene();

    m_ActiveCamera->GetTransformComponent()->GetPosition() = {-28.6f,12.3f,-24.1f};
    m_ActiveCamera->GetTransformComponent()->GetRotation() = {0.0f, 43.0f, 0.0f};
}

void GACPScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::SetNextItemWidth(500);
    ImGui::Begin(GetName().c_str());

    ImGui::SeparatorText("Spawner");
    ImGui::DragFloat("Spawn Height", &GACPSpawner::SpawnHeight, 1.0f, 0.0f, 1000.0f);
    if (ImGui::Button("Spawn Grid"))  m_GridSpawner.SpawnGrid(shared_from_this());
    ImGui::SameLine();
    ImGui::DragInt("Grid Size", &m_GridSpawner.GridSize, 1.0f, 1, 100);

    ImGui::SeparatorText("Job Subsystem");
    m_JobSubsystemWidget->Update(_deltaTime);

    ImGui::End();
}