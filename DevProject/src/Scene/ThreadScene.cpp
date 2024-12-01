#include "ThreadScene.h"

#include "imgui.h"
#include "implot.h"
#include "Denix/Engine.h"
#include "Denix/Thread/JobSubsystem.h"

using namespace  Denix;

ThreadScene::~ThreadScene()
{
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
    ImGui::SeparatorText("Dev Stuff");
    ImGui::DragInt("Grid Size", &m_GridSpawner.GridSize, 1.0f, 1, 100);
    ImGui::SameLine();
    if (ImGui::Button("Spawn Grid"))  m_GridSpawner.SpawnGrid(shared_from_this());

    ImGui::SeparatorText("Engine Thread Settings");
    ImGui::DragInt("Wait For Counter Sleep Time", &Thread::s_WaitForCounterSleepTime);
    ImGui::DragInt("Wait For Job Sleep Time", &Thread::s_WaitForJobSleepTime);
    ImGui::Checkbox("Parallel Loop", RenderParallel.get());
    //ImGui::Checkbox("Dummy Subsystem A", &Engine::Get()->m_DummySubsystemA);
    //ImGui::Checkbox("Dummy Subsystem B", &Engine::Get()->m_DummySubsystemB);
    //ImGui::Checkbox("Parallel Dummy Jobs", &Engine::Get()->m_ParallelDummyJobs);
    ImGui::SeparatorText("Job Subsystem");
    static Ref<JobSubsystem> jobSubsystem = JobSubsystem::Get();
    ImGui::Checkbox("Enabled", &jobSubsystem->IsEnabled());
    ImGui::Checkbox("Construct Lambda Job", &jobSubsystem->m_ConstructLambaJob);
    ImGui::Text("Thread count: %d", jobSubsystem->m_WorkerThreads.size());
    ImGui::Text("System Thread count: %d", jobSubsystem->m_SystemThreadCount);
    ImGui::Separator();
    ImGui::Text("Jobs queued: %d", jobSubsystem->m_Jobs.size());
    ImGui::Text("Jobs done: %d", jobSubsystem->m_JobsDone);

    for (const auto& thread : jobSubsystem->m_WorkerThreads)
    {
        ImVec4 color = thread->m_IsWorking? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) : ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
        ImGui::ColorButton("##ThreadColor", color, ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20));
        if (thread != jobSubsystem->m_WorkerThreads.back()) ImGui::SameLine();
    }
    for (const auto& thread : jobSubsystem->m_WorkerThreads)
    {
        ImGui::Text("Thread ID: %d", thread->m_ThreadIDInt);
        ImGui::Text("Jobs Done: %d", thread->m_JobsDone);
        ImGui::Text("Is Working: %s", thread->m_IsWorking ? "True" : "False");
    }

    ImGui::SeparatorText("Debug Jobs" );
    
    if (ImGui::Button("Add Jobs AB"))
    {
        DebugCounter = MakeRef<Counter>(2);
        for (int i = 0; i < 20; i++) jobSubsystem->AddJob("Test Job A", Priority::NORMAL, DebugCounter,&ThreadScene::JobA, this);
        jobSubsystem->AddJob("Test Job B", Priority::HIGH, DebugCounter,&ThreadScene::JobB, this);
    }

    if (ImGui::Button("Add Jobs A Arg"))
    {
        DebugCounter = MakeRef<Counter>(1);
        jobSubsystem->AddJob("Test Job A", Priority::NORMAL, DebugCounter,&ThreadScene::JobAArg, this, 1);
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
    while (WorkTimer->GetElapsed() < 1.0f)
    {
    }

    WorkTimer->Stop();
    DE_LOG(Log, Info, "Job A Done");
}

void ThreadScene::JobAArg(int _arg)
{
    DE_LOG(Log, Info, "Job A Arg: {0}", _arg);
    Ref<Timer> WorkTimer = MakeRef<Timer>(ObjectInit("Work Timer"));
    WorkTimer->Start();

    // Work for 10 minutes
    while (WorkTimer->GetElapsed() < _arg)
    {
    }

    WorkTimer->Stop();
    
}

void ThreadScene::JobB()
{
    Ref<Timer> WorkTimer = MakeRef<Timer>(ObjectInit("Work Timer"));
    WorkTimer->Start();

    // Work for 10 minutes
    while (WorkTimer->GetElapsed() < 1.0f)
    {
    }

    WorkTimer->Stop();
    DE_LOG(Log, Info, "Job B Done");
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