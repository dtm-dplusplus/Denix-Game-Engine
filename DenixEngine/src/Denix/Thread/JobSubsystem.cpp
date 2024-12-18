#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "JobSubsystem.h"

#include "Denix/Core/TimerSubsystem.h"

Denix::JobSubsystem* Denix::JobSubsystem::s_JobSubsystem = nullptr;

Denix::JobSubsystem::JobSubsystem()
{
    s_JobSubsystem = this;
    m_SystemThreads = 0;
    m_ActiveWorkerThreads = 0;
    m_AvailableWorkerThreads = 0;
    m_AutoBatchingEnabled = true;
    m_ManualBatchSize = 100;
    m_BatchUpdateThreshold = 50;
    DE_LOG_CREATE(LogThread)
    DE_LOG_CREATE(LogJob)
}

Denix::JobSubsystem::~JobSubsystem()
{
    s_JobSubsystem = nullptr;
}

void Denix::JobSubsystem::UpdateActiveThreads()
{
    // Clamp the active threads to the system thread count
    s_JobSubsystem->m_ActiveWorkerThreads = std::clamp(s_JobSubsystem->m_ActiveWorkerThreads, 1,
                                                       s_JobSubsystem->m_AvailableWorkerThreads);
    DE_LOG(LogJob, Trace, "Set Active Worker Threads: {} of {}", s_JobSubsystem->m_ActiveWorkerThreads,
           s_JobSubsystem->m_AvailableWorkerThreads)

    // Update the worker threads
    for (int i = 0; i < s_JobSubsystem->m_AvailableWorkerThreads; i++)
    {
        if (i < s_JobSubsystem->m_ActiveWorkerThreads) s_JobSubsystem->m_WorkerThreads[i]->m_Active = true;
        else s_JobSubsystem->m_WorkerThreads[i]->m_Active = false;
    }
}

Denix::Ref<Denix::JobDeclaration> Denix::JobSubsystem::RequestJob()
{
    Ref<JobDeclaration> job;
    s_JobSubsystem->m_Jobs.try_pop(job);
    return job ? job : nullptr;
}

void Denix::JobSubsystem::StartThreadProfiling()
{
    // Setup Threads
    for (auto& thread : s_JobSubsystem->m_WorkerThreads)
    {
        thread->m_JobExecCount = 0;
        thread->m_ThreadExecTime = 0.0f;
        thread->m_ThreadSleepTime = 0.0f;
    }

    // Set the profiling flag for the threads
    Thread::s_ShouldProfile = true;
}

void Denix::JobSubsystem::StopThreadProfiling()
{
    Thread::s_ShouldProfile = false;

    if (const Ref<ProfileSession> activeProfileSession = ProfileSubsystem::GetActiveProfileSession())
    {
        for (const auto& thread : s_JobSubsystem->m_WorkerThreads)
        {
            activeProfileSession->m_ThreadData.push_back({
                .m_ThreadID= thread->m_ThreadIDInt, .m_JobExecCount= thread->m_JobExecCount, .m_ThreadExecTime= thread->
                m_ThreadExecTime, .m_ThreadSleepTime= thread->m_ThreadSleepTime
            });
        }
    }
    else
        DE_LOG(LogThread, Info, "Thread Profiling: Disabled")
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
    for (int i = 0; i < m_ActiveWorkerThreads; i++)
    {
        m_WorkerThreads.emplace_back(MakeRef<Thread>(i + 1));
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
