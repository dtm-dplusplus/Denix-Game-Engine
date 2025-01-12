#pragma once

#include "Denix/Scene/Scene.h"
#include "GACPSpawner.h"

namespace Denix
{
    class Timer;
    class JobSubsystemWidget;

    class GACPScene final: public Scene
    {
    public:
        GACPScene();
        void BeginPlay() override;
        void Update(float _deltaTime) override;
        void DebugUI(float _deltaTime) override;

        void BeginBenchmark();
        void EndBenchmark();
        
        void BeginTest();
        void EndTest();
        
        // Benchmarking Utilities
        int m_SpawnCount;
        int m_SpawnIncrement;
        int m_SpawnCountMax;

        int m_TestCount;
        int m_TestCountMax;
        float m_TestTimeMax;
        float m_TestCooldownTime;
        Ref<Timer> m_CooldownTimer;
        
        bool m_BenchmarkOnPlay;
        bool m_IsBenchmarking;
        inline static int m_BenchmarkCount = 0;
        
        GACPSpawner m_GridSpawner;
        Ref<JobSubsystemWidget> m_JobSubsystemWidget;
    };
}
