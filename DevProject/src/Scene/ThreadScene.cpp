#include "ThreadScene.h"

#include "imgui.h"
#include "implot.h"
#include "Denix/Engine.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"
#include "CPG/JobSubsystemWidget.h"

using namespace Denix;

ThreadScene::ThreadScene()
{
    m_JobSubsystemWidget = MakeRef<JobSubsystemWidget>(ObjectInit("Job Subsystem Widget"));
}

void ThreadScene::BeginScene()
{
    Scene::BeginScene();
}

void ThreadScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);
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
        if (ImGui::Button("Spawn Grid"))
        {
            m_GridSpawner.SpawnGrid(shared_from_this());
            /*for (auto actor: m_Actors)
            {
                actor->GetPhysicsComponent()->SimulatePhysics() = true;
            }*/
        }

        static float min = 0.0f, max = 100.0f;
        ImGui::DragFloat("Min", &min, 1.0f, 0.0f, 100.0f);
        ImGui::DragFloat("Max", &max, 1.0f, 0.0f, 100.0f);
        if (ImGui::Button("Spawn Random"))
        {
            DE_LOG(Log, Trace,"Random: {}", Math::RandF(min, max))
        }
    }

     m_JobSubsystemWidget->Update(_deltaTime);

        ImGui::End();
}