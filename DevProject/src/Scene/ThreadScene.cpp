#include "ThreadScene.h"

#include "imgui.h"
#include "implot.h"
#include "Denix/Thread/ThreadSubsystem.h"

using namespace  Denix;

ThreadScene::~ThreadScene()
{
    DebugThread.reset();
}

void ThreadScene::BeginScene()
{
    Scene::BeginScene();

    DebugThread = MakeRef<Thread>();
}

void ThreadScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::Begin(GetName().c_str());
    ImGui::SeparatorText("Thread Subsystem");
    static Ref<ThreadSubsystem> ThreadSubsystem = ThreadSubsystem::Get();
    ImGui::Checkbox("Enabled", &ThreadSubsystem->IsEnabled());
    ImGui::Text("Thread count: %d", ThreadSubsystem->m_WorkerThreads.size());
    ImGui::Text("System Thread count: %d", ThreadSubsystem->m_SystemThreadCount);
    ImGui::Separator();
    ImGui::Text("Jobs queued: %d", ThreadSubsystem->m_Jobs.size());
    ImGui::Text("Jobs done: %d", ThreadSubsystem->m_JobsDone);

    for (const auto& thread : ThreadSubsystem->m_WorkerThreads)
    {
        ImVec4 color = thread->m_IsWorking? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) : ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
        ImGui::ColorButton("##ThreadColor", color, ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20));
        if (thread != ThreadSubsystem->m_WorkerThreads.back()) ImGui::SameLine();
    }
    for (const auto& thread : ThreadSubsystem->m_WorkerThreads)
    {
        ImGui::Text("Thread ID: %d", thread->m_ThreadIDInt);
        ImGui::Text("Jobs Done: %d", thread->m_JobsDone);
        ImGui::Text("Is Working: %s", thread->m_IsWorking ? "True" : "False");
    }
   
   
    static int JobSpawn = 1;
    ImGui::DragInt("Job Spawn", &JobSpawn, 1, 1, 1000);
    ImGui::SameLine();
    if (ImGui::Button("Add Jobs"))
    {
        for (int i = 0; i < JobSpawn; i++)
        {
            ThreadSubsystem->AddJob("Test Job", Priority::NORMAL, &ThreadScene::JobA, this);
        }
    }

    if (ImGui::Button("Add Job A"))
    {
        ThreadSubsystem->AddJob("Test Job A", Priority::NORMAL, &ThreadScene::JobA, this);
    }
    
    if (ImGui::Button("Add Job B"))
    {
        ThreadSubsystem->AddJob("Test Job B", Priority::NORMAL, &ThreadScene::JobB, this);
    }

    if (ImGui::Button("Add Job C"))
    {
        ThreadSubsystem->AddJob("Test Job C", Priority::NORMAL, &ThreadScene::JobC, this);
    }

    // Frame Graph
    //Histogram/ ImGui Histogram
    // Error Bar
    // Bar Stacks
    // Bar Groups
    // Bar Plots
    // Scatter
    // Markers

    
    ImGui::End();
}

void ThreadScene::JobA()
{
    Ref<Timer> WorkTimer = MakeRef<Timer>(ObjectInit("Work Timer"));
    WorkTimer->Start();

    // Work for 10 minutes
    while (WorkTimer->GetElapsed() < 2.0f)
    {
    }

    WorkTimer->Stop();
}

void ThreadScene::JobB()
{
    Ref<Timer> WorkTimer = MakeRef<Timer>(ObjectInit("Work Timer"));
    WorkTimer->Start();

    // Work for 10 minutes
    while (WorkTimer->GetElapsed() < 2.0f)
    {
    }

    WorkTimer->Stop();
}

void ThreadScene::JobC()
{
    Ref<Timer> WorkTimer = MakeRef<Timer>(ObjectInit("Work Timer"));
    WorkTimer->Start();

    // Work for 10 minutes
    while (WorkTimer->GetElapsed() < 2.0f)
    {
    }

    WorkTimer->Stop();
}