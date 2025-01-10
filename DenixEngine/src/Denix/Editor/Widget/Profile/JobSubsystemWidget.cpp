#include "JobSubsystemWidget.h"

#include "imgui.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"


namespace Denix
{
    void JobSubsystemWidget::Update(float _deltaTime)
    {
        EditorWidget::Update(_deltaTime);

        static Ref<JobSubsystem> jobSubsystem = JobSubsystem::GetInstance();

        ImGui::Text("System Threads Available: %d", jobSubsystem->GetActiveThreads());
        ImGui::Text("Worker Threads Available: %d", jobSubsystem->GetActiveThreads());
        ImGui::Text("Worker Threads Active: %d", jobSubsystem->GetActiveThreads());
        ImGui::Checkbox("Scene Threaded", &SceneSubsystem::GetInstance()->m_BatchUpdateActors);
        
        if (ImGui::InputInt("Active Threads", &jobSubsystem->GetActiveThreadsRef())) JobSubsystem::UpdateActiveThreads();

       ImGui::SeparatorText("Thread Profiling");
        ImGui::Text("Adjust these values to experiment with thread scheduling");
        ImGui::DragInt("Wait For Counter Sleep Time", &Thread::GetWaitForCounterSleepTime());
        ImGui::DragInt("Wait For Job Sleep Time", &Thread::GetWaitForJobSleepTime());

        ImGui::SeparatorText("Batching");
        ImGui::DragInt("Batch Threshold", &jobSubsystem->GetBatchUpdateThreshold());
    }
}