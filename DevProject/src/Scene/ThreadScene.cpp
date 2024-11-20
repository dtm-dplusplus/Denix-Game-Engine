#include "ThreadScene.h"

#include "imgui.h"
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
    ImGui::Text("Thread count: %d", ThreadSubsystem->m_Threads.size());
    ImGui::Text("System Thread count: %d", ThreadSubsystem->m_SystemThreadCount);
    ImGui::Separator();
    ImGui::Text("Jobs queued: %d", ThreadSubsystem->m_Jobs.size());
    ImGui::Text("Jobs done: %d", ThreadSubsystem->m_JobsDone);

    for (const auto& thread : ThreadSubsystem->m_Threads)
    {
        ImVec4 color = thread->m_IsWorking? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) : ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
        ImGui::ColorButton("##ThreadColor", color, ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20));
        if (thread != ThreadSubsystem->m_Threads.back()) ImGui::SameLine();
    }
    for (const auto& thread : ThreadSubsystem->m_Threads)
    {
        ImGui::Text("Thread ID: %d", thread->m_ThreadIDInt);
        ImGui::Text("Jobs Done: %d", thread->m_JobsDone);
        ImGui::Text("Is Working: %s", thread->m_IsWorking ? "True" : "False");
    }
   
   
    static int JobSpawn = 1;
    ImGui::DragInt("Job Spawn", &JobSpawn, 1, 1, 1000);
    ImGui::SameLine();
    if (ImGui::Button("Add Job"))
    {
        for (int i = 0; i < JobSpawn; i++)
        {
            ThreadSubsystem->AddJob(&ThreadScene::MyWork, this);
        }
    }
    ImGui::End();
}

void ThreadScene::MyWork()
{
    Ref<Thread> myThread = ThreadSubsystem::GetThread(std::this_thread::get_id());
    DE_LOG(LogScene, Info, "Working on ThreadScene thread: {}", myThread->m_ThreadIDInt);
    Ref<Timer> WorkTimer = MakeRef<Timer>(ObjectInit("Work Timer"));
    WorkTimer->Start();

    // Work for 10 minutes
    while (WorkTimer->GetElapsed() < 2.0f)
    {
    }

    WorkTimer->Stop();

    DE_LOG (LogScene, Info, "Work took: {}  on thread {}", WorkTimer->m_Duration.count(), myThread->m_ThreadIDInt);
}
