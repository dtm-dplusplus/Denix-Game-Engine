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
    if (WorkArg) ImGui::Text("Working...");
    else ImGui::Text("Not working...");
    if(ImGui::Button("Start Work"))
    {
        WorkArg = true;
       Threads.push_back(MakeRef<Thread>(&ThreadScene::InfiniteWork, this, std::ref(WorkArg)));
    }
    if(ImGui::Button("Stop Work"))
    {
        WorkArg = false;
        Threads[0]->Join();
    }
    if (ImGui::Button("Detach Work"))
    {
        Threads[0]->Detach();
    }
   
    ImGui::End();

    // join the thread. Either by calling join() or by letting the destructor do it
    /*if (!Threads.empty())
    {
        if (Threads[0]->IsJoinable())
        {
            Threads[0]->Join();
        }
    }*/
}

void ThreadScene::Work(int _arg)
{
    DE_LOG(LogScene, Info, "Working on thread: {}", 1);
    DE_LOG(LogScene, Info, "Work Argument: {}", _arg);
    WorkTimer->Start();

    for (int i = 0; i < 1000000; i++)
    {
        int x = i * i;
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
