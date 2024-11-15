#include "ThreadScene.h"

#include "imgui.h"

using namespace  Denix;

ThreadScene::ThreadScene()
{
    
}

void ThreadScene::BeginScene()
{
    Scene::BeginScene();

    WorkTimer = MakeRef<Timer>(ObjectInit("WorkTimer"));
    /*Ref<Thread> thread = MakeRef<Thread>(Work);
    Threads.push_back(thread);*/

}

void ThreadScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::Begin(GetName().c_str());
    if(ImGui::Button("Start Work"))
    {
       Ref<Thread> thread = MakeRef<Thread>(&ThreadScene::Work, this);
    }

    ImGui::End();
}

void ThreadScene::Work()
{
    DE_LOG(LogScene, Info, "Working on thread: {}", 1);
    WorkTimer->Start();

    for (int i = 0; i < 1000000; i++)
    {
        int x = i * i;
    }

    WorkTimer->Stop();

    DE_LOG (LogScene, Info, "Work took: {} seconds", WorkTimer->m_Duration.count());
}
