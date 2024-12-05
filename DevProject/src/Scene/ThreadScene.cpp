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

    RenderParallel = Engine::Get()->m_ParallelLoop;
}

void ThreadScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);

    // Get the first Actor in the scene and oscillate its position
    //static float moveDir= 1.0f;
    /*if (m_Actors.size() > 0)
    {
        Ref<Actor> actor = m_Actors[0];
        glm::vec3& position = actor->GetTransformComponent()->GetPosition();
        if (position.x > 2.0f) moveDir = -1.0f;
        else if (position.x < -2.0f) moveDir = 1.0f;
        position.x += moveDir * 3.0f * _deltaTime;        
    }*/

    if (DebugCounter) WaitForCounter(DebugCounter.get());
}

void ThreadScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::Begin(GetName().c_str());
    if (ImGui::CollapsingHeader("Dev Stuff", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragInt("Grid Size", &m_GridSpawner.GridSize, 1.0f, 1, 100);
        ImGui::SameLine();
        if (ImGui::Button("Spawn Grid")) m_GridSpawner.SpawnGrid(shared_from_this());
    }

     m_JobSubsystemWidget->Update(_deltaTime);

        ImGui::End();
}