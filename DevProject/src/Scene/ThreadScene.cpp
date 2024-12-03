#include "ThreadScene.h"

#include "imgui.h"
#include "implot.h"
#include "Denix/Engine.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"

using namespace Denix;

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
    if (ImGui::CollapsingHeader("Dev Stuff", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragInt("Grid Size", &m_GridSpawner.GridSize, 1.0f, 1, 100);
        ImGui::SameLine();
        if (ImGui::Button("Spawn Grid")) m_GridSpawner.SpawnGrid(shared_from_this());
    }

    if (ImGui::CollapsingHeader("Job Subsystem"), ImGuiTreeNodeFlags_DefaultOpen)
    {
        static Ref<JobSubsystem> jobSubsystem = JobSubsystem::Get();
        std::vector<Ref<Thread>> threads = jobSubsystem->GetWorkerThreads();

        ImGui::Text("System Threads Available: %d", jobSubsystem->GetActiveThreads());
        ImGui::Text("Worker Threads Available: %d", jobSubsystem->GetActiveThreads());
        ImGui::Text("Worker Threads Active: %d", jobSubsystem->GetActiveThreads());
        ImGui::Text("Jobs queued: %d", jobSubsystem->GetJobQueueSize());

        if (ImGui::InputInt("Active Threads", &jobSubsystem->GetActiveThreadsRef()))
        {
            JobSubsystem::UpdateActiveThreads();
        }        
        if (ImGui::TreeNode("Profiling"))
        {
            if (ImGui::Checkbox("Thread Profiling", &Thread::s_ShouldProfile)) JobSubsystem::ToggleThreadProfiling();

            if (Thread::s_ShouldProfile)
            {
                // Calc Load Difference
                float jobLoadMin = threads[0]->m_JobExecCount, jobLoadMax = 0;
                float execTimeMin = threads[0]->m_ThreadExecTime, execTimeMax = 0;

                for (const auto& thread : threads)
                {
                    if (thread->m_JobExecCount > jobLoadMax) jobLoadMax = thread->m_JobExecCount;
                    if (thread->m_JobExecCount < jobLoadMin) jobLoadMin = thread->m_JobExecCount;

                    if (thread->m_ThreadExecTime > execTimeMax) execTimeMax = thread->m_ThreadExecTime;
                    if (thread->m_ThreadExecTime < execTimeMin) execTimeMin = thread->m_ThreadExecTime;
                }

                float jobLoadDiff = 1.0f - jobLoadMin / jobLoadMax;
                float execTimeDiff = execTimeMin / execTimeMax;
                ImGui::Text("Load Diff: %f%", jobLoadDiff * 100.0f);
                ImGui::Text("Exec Diff: %f%", execTimeDiff * 100.0f);

                for (const auto& thread : threads)
                {
                    ImGui::Text("ID: %d, Job Executed: %d, Thread Exec Time: %f, Thread Sleep Time: %f",
                                thread->m_ThreadIDInt, thread->m_JobExecCount, thread->m_ThreadExecTime,
                                thread->m_ThreadSleepTime);
                }
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Thread Settings"))
        {
            ImGui::DragInt("Wait For Counter Sleep Time", &Thread::s_WaitForCounterSleepTime);
            ImGui::DragInt("Wait For Job Sleep Time", &Thread::s_WaitForJobSleepTime);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Batch Settings"))
        {
            ImGui::DragInt("Batch Size", &SceneSubsystem::Get()->m_SceneBatchSize);
            ImGui::Checkbox("Batch Update", &SceneSubsystem::Get()->m_BatchUpdate);
            ImGui::Text("Batch Count: %d", SceneSubsystem::Get()->m_SceneBatchCount);
            ImGui::TreePop();
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
