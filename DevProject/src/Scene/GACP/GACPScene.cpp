#include "GACPScene.h"

#include "imgui.h"
#include "Denix/Engine.h"
#include "Denix/Core/Time/TimerSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Core/Thread/JobSubsystem.h"
#include "Denix/Editor/Widget/Profile/JobSubsystemWidget.h"

using namespace Denix;

GACPScene::GACPScene():
    m_SpawnCount(0), m_SpawnIncrement(10), m_SpawnCountMax(100), m_TestVariant(0), m_TestVariantMax(0), m_TestCount(0),
    m_TestCountMax(0),
    m_TestTimeMax(15), m_TestCooldownTime(2),
    m_CooldownTimer(MakeRef<Timer>()), m_CoreCount(0), m_CoreIncrement(2), m_CoreCountMax(0),
    m_BenchmarkOnPlay(true),
    m_IsBenchmarking(false),
    m_JobSubsystemWidget(MakeRef<JobSubsystemWidget>(ObjectInit("Job Subsystem Widget")))
{
    m_ActiveCamera->GetTransformComponent()->GetPosition() = {-28.6f, 12.3f, -24.1f};
    m_ActiveCamera->GetTransformComponent()->GetRotation() = {0.0f, 43.0f, 0.0f};
}

void GACPScene::BeginPlay()
{
    Scene::BeginPlay();

    if (m_BenchmarkOnPlay) BeginBenchmark();
}

void GACPScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);

    if (m_IsBenchmarking && IsPlaying())
    {
        // Check if we have reached the max spawn count & finished both test types
        if (m_TestCount >= m_TestCountMax)
        {
            EndBenchmark();
            return;
        }

        // Check cooldown before running the test - Allow framerate to stabilize
        if (m_CooldownTimer->GetElapsed() < m_TestCooldownTime) return;

        // Start the test, End if the test time is reached
        if (const Ref<ProfileSession> test = ProfileSubsystem::GetActiveProfileSession())
        {
            if (test->GetSessionTimer()->GetElapsed() >= m_TestTimeMax) EndTest();
        }
        else
        {
            BeginTest();
        }
    }
}

void GACPScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::SetNextItemWidth(500);
    ImGui::Begin(GetName().c_str());
    ImGui::SeparatorText("Benchmarking");
    
    if (m_IsBenchmarking)
    {
        if (ImGui::Button("Force stop Benchmark")) EndBenchmark();
        ImGui::Text("Test: %d / %d", m_TestCount, m_TestCountMax);
    }
    else
    {
        if (!m_BenchmarkOnPlay && IsPlaying())if (ImGui::Button("Benchmark")) BeginBenchmark();
        ImGui::Checkbox("Benchmark On Play", &m_BenchmarkOnPlay);
        ImGui::DragFloat("Test Time Max", &m_TestTimeMax, 1.0f, 1.0f, 100.0f);
        ImGui::DragFloat("Test Cooldown Time", &m_TestCooldownTime, 0.1f, 0.0f, 10.0f);
        ImGui::SeparatorText("Spawner");
        ImGui::Text("Spawn Count: %d", m_SpawnCount);
        ImGui::DragInt("Spawn Max", &m_SpawnCountMax, 1.0f, m_SpawnIncrement, 100);
        ImGui::DragInt("Spawn Increment", &m_SpawnIncrement, 1.0f, 1, m_SpawnCountMax);
        ImGui::DragFloat("Spawn Height", &GACPSpawner::SpawnHeight, 1.0f, 0.0f, 1000.0f);
        if (ImGui::Button("Spawn Grid")) m_GridSpawner.SpawnGrid(shared_from_this());
        ImGui::SameLine();
        ImGui::DragInt("Grid Size", &m_GridSpawner.GridSize, 1.0f, 1, m_SpawnCountMax);

        ImGui::SeparatorText("Job Subsystem");
        m_JobSubsystemWidget->Update(_deltaTime);
    }

    ImGui::End();
}

void GACPScene::BeginTest()
{
    // Incremenet for first test type and spawn grid
    if (m_TestVariant == 0)
    {
        m_SpawnCount += m_SpawnIncrement;
        m_GridSpawner.GridSize = m_SpawnCount;
        m_GridSpawner.SpawnGrid(shared_from_this());

        SceneSubsystem::m_BatchUpdateActors = false;
    }
    else
    {
        SceneSubsystem::m_BatchUpdateActors = true;
        JobSubsystem::GetActiveThreadsRef() = m_CoreCount;
        JobSubsystem::UpdateActiveThreads();
    }

    std::string variant = m_TestVariant == 0 ? "Single" : "Multi_" + std::to_string(m_CoreCount);
    
    //if ()
    const std::string name = "GACP BM: " + std::to_string(m_BenchmarkCount) +
        " Test: " + std::to_string(m_TestCount) + " " + variant +
        " Thread Grid Size: " + std::to_string(m_SpawnCount) +
        " Actors: " + std::to_string((int)pow(m_SpawnCount, 2));

    ProfileSubsystem::StartProfileSession(name);
}

void GACPScene::EndTest()
{
    ProfileSubsystem::EndProfileSession();
    m_CooldownTimer->Start();

    m_CoreCount += m_CoreIncrement;

    // Check if we have finished our test variants
    if (m_TestVariant++ == m_TestVariantMax -1)
    {
        m_TestVariant = 0;
        m_CoreCount = 0;
        m_TestCount++;
    }
}

void GACPScene::BeginBenchmark()
{
    m_IsBenchmarking = true;
    m_TestCount = 0;
    m_TestCountMax = m_SpawnCountMax / m_SpawnIncrement;
    m_SpawnCount = 0;
    m_TestVariant = 0;
    m_TestVariantMax = 1 + JobSubsystem::GetAvailableThreads() /m_CoreIncrement;
    m_BenchmarkCount++;

    // Uncap the frame rate
    TimerSubsystem::SetMaxFPS(0);

    float tests = m_TestCountMax  * m_TestVariantMax;
    float eta = m_TestCooldownTime * tests +  tests * m_TestTimeMax;
    DE_LOG(LogDevProject, Warn, "Benchmarking Started")
    DE_LOG(LogDevProject, Warn, "Estimated Time to Complete: {}s", eta)
}

void GACPScene::EndBenchmark()
{
    m_IsBenchmarking = false;

    // Restore Threads
    JobSubsystem::GetActiveThreadsRef() = JobSubsystem::GetSystemThreads();
    JobSubsystem::UpdateActiveThreads();
    
    DE_LOG(LogDevProject, Info, "Benchmarking Finished")
}
