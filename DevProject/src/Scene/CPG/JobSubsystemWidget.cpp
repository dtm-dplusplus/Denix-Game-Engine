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

        ImGui::Text("System Threads Available: %d", jobSubsystem->GetActiveThreads());
        ImGui::Text("Worker Threads Available: %d", jobSubsystem->GetActiveThreads());
        ImGui::Text("Worker Threads Active: %d", jobSubsystem->GetActiveThreads());
        ImGui::Checkbox("Scene Threaded", &SceneSubsystem::GetInstance()->m_BatchUpdateActors);
        
        if (ImGui::InputInt("Active Threads", &jobSubsystem->GetActiveThreadsRef())) JobSubsystem::UpdateActiveThreads();

       ImGui::SeparatorText("Thread Profiling");
        ImGui::DragInt("Wait For Counter Sleep Time", &Thread::s_WaitForCounterSleepTime);
        ImGui::DragInt("Wait For Job Sleep Time", &Thread::s_WaitForJobSleepTime);

        ImGui::SeparatorText("Batching");
        ImGui::Checkbox("Auto Batching", &JobSubsystem::IsAutoBatchingEnabled());
        ImGui::Text("Batch Count: %d", JobSubsystem::GetBatchSize());
        ImGui::DragInt("Batch Threshold", &jobSubsystem->GetBatchUpdateThreshold());
    }
}