#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "JobSubsystem.h"

#include "Denix/Core/TimerSubsystem.h"

Denix::JobSubsystem* Denix::JobSubsystem::s_ThreadSubsystem = nullptr;

void Denix::JobSubsystem::Initialize()
{
    Subsystem::Initialize();

    DE_LOG(Log, Warn, "Thread Subsystem Initializing")
    // Get Thread Information
    m_SystemThreadCount = std::thread::hardware_concurrency();
    
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    DE_LOG(Log, Trace, "Number of cores: {}", sysinfo.dwNumberOfProcessors)
    DE_LOG(Log, Trace, "Number of threads: {}", m_SystemThreadCount)
    DE_LOG(Log, Trace, "Page size: {}", sysinfo.dwPageSize)
    DE_LOG(Log, Trace, "Processor type: {}", sysinfo.dwProcessorType)
    DE_LOG(Log, Trace, "Minimum application address: {}", sysinfo.lpMinimumApplicationAddress)
    DE_LOG(Log, Trace, "Maximum application address: {}", sysinfo.lpMaximumApplicationAddress)
    DE_LOG(Log, Trace, "Active processor mask: {}", sysinfo.dwActiveProcessorMask)
    DE_LOG(Log, Trace, "Allocation granularity: {}", sysinfo.dwAllocationGranularity)
    DE_LOG(Log, Trace, "Processor architecture: {}", sysinfo.wProcessorArchitecture)

    // Initialize the scheduler thread
   m_ThreadScheduler = MakeRef<Thread>();
    m_ThreadScheduler->InitThread(&JobSubsystem::ScheduleWork, this);
    
    // Initialize the worker threads
    for (size_t i = 0; i < m_SystemThreadCount - 3; i++)
    {
        m_WorkerThreads.emplace_back(MakeRef<Thread>());
        m_WorkerThreads.back()->InitWorkerThread();
    }
    DE_LOG(Log, Info, "Thread Subsystem Initialized")
}

void Denix::JobSubsystem::Deinitialize()
{
    Subsystem::Deinitialize();

    DE_LOG(Log, Trace, "Thread Subsystem Deinitializing")
    DE_LOG(Log, Trace, "Thread Subsystem Deinitialized")
}

void Denix::JobSubsystem::ScheduleWork()
{
    while (true)
    {
        // Probably need a lock here?
        if (!m_Jobs.empty())
        {
            for (const auto& thread : m_WorkerThreads)
            {
                if (!thread->m_IsWorking)
                {
                    thread->m_IsWorking = true;
                    thread->m_Job = m_Jobs.top();
                    m_Jobs.pop();
                    break;
                }
            }
        }

        // Sleep for a bit
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}