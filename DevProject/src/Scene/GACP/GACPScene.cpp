#include "GACPScene.h"

#include "imgui.h"
#include "Denix/Engine.h"
#include "Denix/Core/TimerSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"
#include "Denix/Editor/Widget/Profile/JobSubsystemWidget.h"

using namespace Denix;

GACPScene::GACPScene():
    m_SpawnCount(0), m_SpawnIncrement(25), m_SpawnCountMax(50), m_TestCount(0), m_TestCountMax(0),
    m_TestTimeMax(20), m_TestCooldownTime(3),
    m_CooldownTimer(MakeRef<Timer>()),
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
        ImGui::Text("Test Count: %d / %d", m_TestCount, m_TestCountMax);
    }

    if (!m_IsBenchmarking)
    {
        if (!m_BenchmarkOnPlay && IsPlaying())if (ImGui::Button("Benchmark")) BeginBenchmark();
        ImGui::Checkbox("Benchmark On Play", &m_BenchmarkOnPlay);
        ImGui::DragFloat("Test Time Max", &m_TestTimeMax, 1.0f, 1.0f, 100.0f);
        ImGui::DragFloat("Test Cooldown Time", &m_TestCooldownTime, 0.1f, 0.0f, 10.0f);
        ImGui::SeparatorText("Spawner");
        ImGui::Text("Spawn Count: %d", m_SpawnCount);
        ImGui::DragInt("Spawn Max", &m_SpawnCountMax, 1.0f, 1, 100);
        ImGui::DragInt("Spawn Increment", &m_SpawnIncrement, 1.0f, 1, m_SpawnCountMax);
        ImGui::DragFloat("Spawn Height", &GACPSpawner::SpawnHeight, 1.0f, 0.0f, 1000.0f);
        if (ImGui::Button("Spawn Grid")) m_GridSpawner.SpawnGrid(shared_from_this());
        ImGui::SameLine();
        ImGui::DragInt("Grid Size", &m_GridSpawner.GridSize, 1.0f, 1, m_SpawnCountMax);
    }

    ImGui::SeparatorText("Job Subsystem");
    m_JobSubsystemWidget->Update(_deltaTime);

    ImGui::End();
}

void GACPScene::BeginTest()
{
    // Incremenet for first test type and spawn grid - Threaded
    if (SceneSubsystem::m_BatchUpdateActors)
    {
        m_SpawnCount += m_SpawnIncrement;
        m_GridSpawner.GridSize = m_SpawnCount;
        m_GridSpawner.SpawnGrid(shared_from_this());
    }

    const std::string name = "GACP BM: " + std::to_string(m_BenchmarkCount) +
        " Test: " + std::to_string(m_TestCount) +
        (SceneSubsystem::m_BatchUpdateActors ? "_Multi" : "_Single") +
        "Thread Grid Size: " + std::to_string(m_SpawnCount) +
        " Actors: " + std::to_string((int)pow(m_SpawnCount, 2));
    
    ProfileSubsystem::StartProfileSession(name);
}

void GACPScene::EndTest()
{
    ProfileSubsystem::EndProfileSession();
    m_CooldownTimer->Start();
    SceneSubsystem::m_BatchUpdateActors = !SceneSubsystem::m_BatchUpdateActors;
    m_TestCount++;
}

void GACPScene::BeginBenchmark()
{
    m_IsBenchmarking = true;
    m_TestCount = 0;
    m_TestCountMax = 2 * (m_SpawnCountMax / m_SpawnIncrement);
    m_SpawnCount = 0;
    m_BenchmarkCount++;
    
    // Uncap the frame rate
    TimerSubsystem::SetMaxFPS(0);

    DE_LOG(LogDevProject, Warn, "Benchmarking Started")
}

void GACPScene::EndBenchmark()
{
    m_IsBenchmarking = false;
    DE_LOG(LogDevProject, Info, "Benchmarking Finished")
}
