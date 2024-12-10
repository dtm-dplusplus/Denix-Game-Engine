#include "JobSubsystemWidget.h"

#include "imgui.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"


namespace Denix
{
    void JobSubsystemWidget::Update(float _deltaTime)
    {
        EditorWidget::Update(_deltaTime);

        static Ref<JobSubsystem> jobSubsystem = JobSubsystem::Get();
        std::vector<Ref<Thread>> threads = jobSubsystem->GetWorkerThreads();
        if (ImGui::CollapsingHeader("Job Subsystem"), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::Text("System Threads Available: %d", jobSubsystem->GetActiveThreads());
            ImGui::Text("Worker Threads Available: %d", jobSubsystem->GetActiveThreads());
            ImGui::Text("Worker Threads Active: %d", jobSubsystem->GetActiveThreads());
            ImGui::Text("Jobs queued: %d", jobSubsystem->GetJobQueueSize());
            ImGui::Checkbox("Scene Threaded", &SceneSubsystem::Get()->m_SceneThreaded);
            
            if (ImGui::InputInt("Active Threads", &jobSubsystem->GetActiveThreadsRef()))
            {
                JobSubsystem::UpdateActiveThreads();
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