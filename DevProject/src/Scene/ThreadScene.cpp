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
    ImGui::Text("Thread count: %d", ThreadSubsystem->m_Threads.size());
    ImGui::Text("System Thread count: %d", ThreadSubsystem->m_SystemThreadCount);
    ImGui::Separator();
    ImGui::Text("Jobs queued: %d", ThreadSubsystem->m_Jobs.size());
    ImGui::Text("Jobs done: %d", ThreadSubsystem->m_JobsDone);
    
    for (auto& thread : ThreadSubsystem->m_Threads)
    {
        ImGui::Text("Thread ID: %d", thread.m_ThreadIDInt);
        ImGui::Text("Jobs Done: %d", thread.m_JobsDone);
        ImGui::Text("Is Working: %s", thread.m_IsWorking ? "True" : "False");
    }
   
    if(ImGui::Button("Add Job"))
    {
      ThreadSubsystem::AddJob(&ThreadScene::MyWork, this);
    }

    ImGui::End();
}

void ThreadScene::MyWork()
{
    DE_LOG(LogScene, Info, "Working on ThreadScene thread: {}", DebugThread->m_ThreadIDInt);
    Ref<Timer> WorkTimer = MakeRef<Timer>(ObjectInit("Work Timer"));
    WorkTimer->Start();

    // Work for 10 minutes
    while (WorkTimer->GetElapsed() < 2.0f)
    {
    }

    WorkTimer->Stop();

    DE_LOG (LogScene, Info, "Work took: {} seconds", WorkTimer->m_Duration.count());
}
