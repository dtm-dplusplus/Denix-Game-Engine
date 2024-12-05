#include "JobSubsystemWidget.h"

#include "imgui.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"

namespace Denix
{
    void JobSubsystemWidget::Update(float _deltaTime)
    {
        EditorWidget::Update(_deltaTime);

        static std::vector<Ref<JobDeclaration>>& jobProfiles = JobSubsystem::Get()->JobProfileBuffer;
        static Ref<JobSubsystem> jobSubsystem = JobSubsystem::Get();
        std::vector<Ref<Thread>> threads = jobSubsystem->GetWorkerThreads();
        if (ImGui::CollapsingHeader("Job Subsystem"), ImGuiTreeNodeFlags_DefaultOpen)
        {
            

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
        }
    }
}
