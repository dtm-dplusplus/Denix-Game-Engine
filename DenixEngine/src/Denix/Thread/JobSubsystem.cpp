#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "JobSubsystem.h"

#include "Denix/Core/TimerSubsystem.h"

Denix::JobSubsystem* Denix::JobSubsystem::s_JobSubsystem = nullptr;

Denix::Ref<Denix::JobDeclaration> Denix::JobSubsystem::RequestJob()
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
    if (const int threadCount = std::thread::hardware_concurrency(); threadCount > 0)
    {
        m_SystemThreads = threadCount;
        m_AvailableWorkerThreads = m_SystemThreads - 1;
        m_ActiveWorkerThreads = m_AvailableWorkerThreads;
    }
    else
    {
        static auto error = "Failed to get system thread count";
        DE_LOG(LogJob, Critical, error)
        throw std::runtime_error(error);
    }


    // Initialize the worker threads - Subtract 1 for main thread
    for (size_t i = 0; i < m_ActiveWorkerThreads; i++)
    {
        m_WorkerThreads.emplace_back(MakeRef<Thread>());
        m_WorkerThreads.back()->InitWorkerThread();
    }

    DE_LOG(LogJob, Trace, "System threads: {}", m_SystemThreads)
    DE_LOG(LogJob, Trace, "Worker threads: {}", m_ActiveWorkerThreads)
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