#include "ThreadScene.h"

#include "imgui.h"
#include "implot.h"
#include "Denix/Engine.h"
#include "Denix/Scene/SceneSubsystem.h"
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
    {
        ImGui::DragInt("Grid Size", &m_GridSpawner.GridSize, 1.0f, 1, 100);
        ImGui::SameLine();
        if (ImGui::Button("Spawn Grid"))  m_GridSpawner.SpawnGrid(shared_from_this());
    }

    std::vector<Ref<Thread>> threads = JobSubsystem::GetWorkerThreads();
    ImGui::SeparatorText("Engine Thread Settings");
    {
        ImGui::DragInt("Wait For Counter Sleep Time", &Thread::s_WaitForCounterSleepTime);
        ImGui::DragInt("Wait For Job Sleep Time", &Thread::s_WaitForJobSleepTime);
       if (ImGui::Button("Toggle Thread Profiling")) JobSubsystem::ToggleThreadProfiling();
        ImGui::SameLine();
        ImGui::Text("Profile Threads %s ", Thread::s_ShouldProfile? "Enabled" : "Disabled");
        ImGui::DragInt("Batch Size", &SceneSubsystem::Get()->m_SceneBatchSize);
        ImGui::Checkbox("Batch Update", &SceneSubsystem::Get()->m_BatchUpdate);
        ImGui::Text("Batch Count: %d", SceneSubsystem::Get()->m_SceneBatchCount);
        ImGui::Checkbox("Parallel Loop", RenderParallel.get());
        //ImGui::Checkbox("Dummy Subsystem A", &Engine::Get()->m_DummySubsystemA);
        //ImGui::Checkbox("Dummy Subsystem B", &Engine::Get()->m_DummySubsystemB);
        //ImGui::Checkbox("Parallel Dummy Jobs", &Engine::Get()->m_ParallelDummyJobs);
    }
    
   
    ImGui::SeparatorText("Job Subsystem");
    static Ref<JobSubsystem> jobSubsystem = JobSubsystem::Get();
    ImGui::Checkbox("Enabled", &jobSubsystem->IsEnabled());
    ImGui::Text("Threads Available: %d", jobSubsystem->GetActiveThreads());
    ImGui::Text("Threads Active: %d", jobSubsystem->GetActiveThreads());
    ImGui::Separator();
    ImGui::Text("Jobs queued: %d", jobSubsystem->GetJobQueueSize());

    if (Thread::s_ShouldProfile)
    {
        // Calc Load Difference
        float jobLoadMin = 0, jobLoadMax = 0;
        float execTimeMin = 0, execTimeMax = 0;
        
        for (const auto& thread : jobSubsystem->GetWorkerThreads())
        {
            if (thread->m_JobExecCount > jobLoadMax) jobLoadMax = thread->m_JobExecCount;
            if (thread->m_JobExecCount < jobLoadMin) jobLoadMin = thread->m_JobExecCount;

            if (thread->m_ThreadExecTime > execTimeMax) execTimeMax = thread->m_ThreadExecTime;
            if (thread->m_ThreadExecTime < execTimeMin) execTimeMin = thread->m_ThreadExecTime;
        }
        
        float jobLoadDiff = jobLoadMin / jobLoadMax;
        float execTimeDiff = execTimeMin / execTimeMax;
        ImGui::Text("Load Min: %d Max: %d Diff: %f", jobLoadMin, jobLoadMax, jobLoadDiff);
        ImGui::Text("Exec Time Min: %f Max: %f Diff: %f", execTimeMin, execTimeMax, execTimeDiff);
        
        // Load Balancing
        /*{
            ImPlotFlags flags = ImPlotFlags_None;
            ImPlotAxisFlags xFlags = ImPlotAxisFlags_AutoFit;
            ImPlotAxisFlags yFlags = ImPlotAxisFlags_AutoFit;
            ImPlot::BeginPlot("Load Balancing", "Thread", "Jobs Exexcuted", ImVec2(-1, 0), flags, xFlags, yFlags);
            std::vector<size_t> jobExecData;
            for (auto t: threads) jobExecData.push_back(t->m_JobExecCount);
            ImPlot::PlotBars("Vertical",&jobExecData[0],jobExecData.size(),0.7);
            ImPlot::EndPlot();
        }*/

        // Exec Time
        {
            ImPlotFlags flags = ImPlotFlags_None;
            ImPlotAxisFlags xFlags = ImPlotAxisFlags_AutoFit;
            ImPlotAxisFlags yFlags = ImPlotAxisFlags_AutoFit;
            ImPlot::BeginPlot("Execution Time", "Thread", "Execution Time", ImVec2(-1, 0), flags, xFlags, yFlags);
            std::vector<size_t> jobExecData;
            for (auto t: threads) jobExecData.push_back(t->m_ThreadExecTime);
            ImPlot::PlotBars("Vertical",&jobExecData[0],jobExecData.size(),0.7);
            ImPlot::EndPlot();
        }
    }
    
    for (const auto& thread : jobSubsystem->GetWorkerThreads())
    {
        ImGui::Text("Thread ID: %d", thread->m_ThreadIDInt);
        if (Thread::s_ShouldProfile)
        {
            ImGui::Text("Jobs Executed: %d", thread->m_JobExecCount);
            ImGui::Text("Thread Execution Time: %f", thread->m_ThreadExecTime);
            ImGui::Text("Thread Sleep Time: %f", thread->m_ThreadSleepTime);
        }
    }

    if (ImGui::CollapsingHeader("Debug Jobs" ))
    {
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