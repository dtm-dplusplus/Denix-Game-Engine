#include "ThreadScene.h"

#include "imgui.h"

using namespace  Denix;

ThreadScene::ThreadScene()
{
    DebugThread = MakeRef<Thread>(&ThreadScene::InfiniteWork, this, std::ref(WorkArg));
    WorkTimer = MakeRef<Timer>(ObjectInit("WorkTimer"));
}

ThreadScene::~ThreadScene()
{
    DebugThread.reset();
    WorkTimer.reset();
}

void ThreadScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::Begin(GetName().c_str());
    if (DebugThread)
    {
        ImGui::Text("Thread ID: %d", DebugThread->m_ThreadIDInt);
        ImGui::Text("Is Working: %s", DebugThread->m_IsWorking ? "True" : "False");
    }
    
    if(ImGui::Button("Start Work"))
    {
       DebugThread = MakeRef<Thread>(&ThreadScene::Work, this);
    }
    if(ImGui::Button("Stop Work"))
    {
        DebugThread->m_StopFlag = true;
    }
    if (ImGui::Button("Join"))
    {
        DebugThread->JoinCheck();
    }
    if (ImGui::Button("Detach"))
    {
        DebugThread->Detach();
    }

    ImGui::End();
}

void ThreadScene::Work()
{
    DE_LOG(LogScene, Info, "Working on thread: {}", DebugThread->m_ThreadIDInt);
    WorkTimer->Start();

    // Work for 10 minutes
    while (WorkTimer->m_Duration.count() < 60.0f * 10.0f && !DebugThread->m_StopFlag)
    {
    }

    WorkTimer->Stop();

    DE_LOG (LogScene, Info, "Work took: {} seconds", WorkTimer->m_Duration.count());
}

void ThreadScene::InfiniteWork(bool& _ShouldWork)
{
    DE_LOG(LogScene, Info, "Working on thread: {}", 1);
    WorkTimer->Start();

    while (_ShouldWork)
    {
    }

    WorkTimer->Stop();

    DE_LOG(LogScene, Info, "Work took: {} seconds", WorkTimer->m_Duration.count());
}
