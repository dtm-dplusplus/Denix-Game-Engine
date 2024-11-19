#include "ThreadScene.h"

#include "imgui.h"

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
    if (DebugThread)
    {
        ImGui::Text("Thread ID: %d", DebugThread->m_ThreadIDInt);
        ImGui::Text("Jobs Done: %d", DebugThread->m_JobsDone);
        ImGui::Text("Is Working: %s", DebugThread->m_IsWorking ? "True" : "False");
    }
    if(ImGui::Button("Add Job"))
    {
        DebugThread->AddJob(&ThreadScene::MyWork, this);
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
