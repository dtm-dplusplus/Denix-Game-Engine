#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "JobSubsystem.h"

#include "Denix/Core/TimerSubsystem.h"

Denix::JobSubsystem* Denix::JobSubsystem::s_JobSubsystem = nullptr;

Denix::Ref<Denix::JobDeclaration> Denix::JobSubsystem::GetJob()
{
    Ref<JobDeclaration> job;
    s_JobSubsystem->m_Jobs.try_pop(job);
    return job? job : nullptr;
}

void Denix::JobSubsystem::Initialize()
{
    Subsystem::Initialize();

    DE_LOG(LogJob, Warn, "Thread Subsystem Initializing")
    // Get Thread Information
    m_SystemThreadCount = std::thread::hardware_concurrency();
    if (m_SystemThreadCount == 0)
    {
        DE_LOG(LogJob, Error, "Failed to get system thread count")
        m_SystemThreadCount = 1;
    }
    

    // Initialize the worker threads - Subtract 1 for main thread
    for (size_t i = 0; i < m_SystemThreadCount - 1; i++)
    {
        m_WorkerThreads.emplace_back(MakeRef<Thread>());
        m_WorkerThreads.back()->InitWorkerThread();
    }

    DE_LOG(LogJob, Trace, "Number of System threads: {}", m_SystemThreadCount)
    DE_LOG(LogJob, Trace, "Initialized {} worker threads", m_WorkerThreads.size())
    DE_LOG(LogJob, Info, "Thread Subsystem Initialized")
}

void Denix::JobSubsystem::Deinitialize()
{
    Subsystem::Deinitialize();

    m_Jobs.clear();
    m_WorkerThreads.clear();
    /*for (auto thread : m_WorkerThreads)
    {
        thread->m_ShouldWork = false;
        thread->JoinCheck();
    }*/
    DE_LOG(LogJob, Trace, "Thread Subsystem Deinitializing")
    DE_LOG(LogJob, Trace, "Thread Subsystem Deinitialized")
}