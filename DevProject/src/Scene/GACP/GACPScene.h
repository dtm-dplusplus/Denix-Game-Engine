#pragma once

#include "Denix/Scene/Scene.h"
#include "GACPSpawner.h"

namespace Denix
{
    class Timer;
    class JobSubsystemWidget;

    /**
     * \brief GACPScene class for managing the GACP scene.
     * Inherits from Scene.
     */
    class GACPScene final: public Scene
    {
    public:
        /**
         * \brief Constructor for GACPScene.
         */
        GACPScene();

        /**
         * \brief Called when the scene begins play.
         */
        void BeginPlay() override;

        /**
         * \brief Updates the scene.
         * \param _deltaTime Time elapsed since the last update.
         */
        void Update(float _deltaTime) override;

        /**
         * \brief Displays the debug UI for the scene.
         * \param _deltaTime Time elapsed since the last update.
         */
        void DebugUI(float _deltaTime) override;

        /**
         * \brief Begins the benchmarking process.
         */
        void BeginBenchmark();

        /**
         * \brief Ends the benchmarking process.
         */
        void EndBenchmark();

        /**
         * \brief Begins a test during benchmarking.
         */
        void BeginTest();

        /**
         * \brief Ends a test during benchmarking.
         */
        void EndTest();

        // Benchmarking Utilities
        int m_SpawnCount; ///< Current spawn count.
        int m_SpawnIncrement; ///< Increment for spawn count.
        int m_SpawnCountMax; ///< Maximum spawn count.

        int m_TestVariant; ///< Current test variant.
        int m_TestVariantMax; ///< Maximum test variant.

        int m_TestCount; ///< Current test count.
        int m_TestCountMax; ///< Maximum test count.
        float m_TestTimeMax; ///< Maximum test time.
        float m_TestCooldownTime; ///< Cooldown time between tests.
        Ref<Timer> m_CooldownTimer; ///< Timer for cooldown period.

        int m_CoreCount; ///< Current core count.
        int m_CoreIncrement; ///< Increment for core count.
        int m_CoreCountMax; ///< Maximum core count.

        bool m_BenchmarkOnPlay; ///< Flag to indicate if benchmarking should start on play.
        bool m_IsBenchmarking; ///< Flag to indicate if benchmarking is active.
        inline static int m_BenchmarkCount = 0; ///< Benchmark count.

        GACPSpawner m_GridSpawner; ///< Spawner for grid objects.
        Ref<JobSubsystemWidget> m_JobSubsystemWidget; ///< Widget for job subsystem.
    };
}