#pragma once
#include "Denix/System/Subsystem.h"

#include "Denix/Thread/Thread.h"
#include <concurrent_priority_queue.h>

namespace Denix
{
    class JobSubsystem : public Subsystem, public std::enable_shared_from_this<JobSubsystem>
    {
    public:
        JobSubsystem()
        {
            s_JobSubsystem = this;
            m_SystemThreads = 0;
            //m_JobExecCount = 0;
            DE_LOG_CREATE(LogThread)
            DE_LOG_CREATE(LogJob)
        }

        ~JobSubsystem() override
        {
            s_JobSubsystem = nullptr;
        }

        // Delete Copy and Move Constructors. Ensure only one instance of JobSubsystem
        JobSubsystem(const JobSubsystem& _other) = delete;
        JobSubsystem(JobSubsystem&& _other) noexcept = delete;
        JobSubsystem& operator=(const JobSubsystem& _other) = delete;
        JobSubsystem& operator=(JobSubsystem&& _other) noexcept = delete;

        /**
         * 
         * @tparam Func 
         * @tparam Args 
         * @param _name 
         * @param _priority 
         * @param _waitCounter
         * @param _func 
         * @param _args 
         */
        template <typename Func, typename... Args>
        static void AddJob(const std::string& _name, const Priority _priority, const Ref<Counter>& _waitCounter,
                           Func&& _func, Args&&... _args)
        {
            Ref<JobDeclaration> job = MakeRef<JobDeclaration>(
                _name,
                _priority,
                _waitCounter ? _waitCounter : MakeRef<Counter>(1),
                std::bind(std::forward<Func>(_func), std::forward<Args>(_args)...));

            s_JobSubsystem->JobProfileBuffer.push_back(job);
            s_JobSubsystem->m_Jobs.push(job);
        }

       static void ToggleThreadProfiling()
        {
            // Clear the profiling data
            for (auto& thread : s_JobSubsystem->m_WorkerThreads)
            {
                thread->m_JobExecCount = 0;
                thread->m_ThreadExecTime = 0.0f;
                thread->m_ThreadSleepTime = 0.0f;
            }

            // Toggle the profiling. The Thread::Work() function will profile the thread if this is true
            DE_LOG(LogThread, Info, "Thread Profiling: {}", Thread::s_ShouldProfile? "Enabled" : "Disabled")
        }

        static void UpdateActiveThreads()
        {
            // Clamp the active threads to the system thread count
            s_JobSubsystem->m_ActiveWorkerThreads = std::clamp(s_JobSubsystem->m_ActiveWorkerThreads, 1, s_JobSubsystem->m_AvailableWorkerThreads);
            DE_LOG(LogJob, Trace, "Set Active Worker Threads: {}", s_JobSubsystem->m_ActiveWorkerThreads)
            DE_LOG(LogJob, Trace, "Available {}", s_JobSubsystem->m_AvailableWorkerThreads)
            
            // Update the worker threads
            for (int i = 0; i < s_JobSubsystem->m_AvailableWorkerThreads; i++)
            {
                if (i < s_JobSubsystem->m_ActiveWorkerThreads)
                {
                    s_JobSubsystem->m_WorkerThreads[i]->m_Active = true;
                }
                else
                {
                    s_JobSubsystem->m_WorkerThreads[i]->m_Active = false;
                }
            }
        }
        struct JobComparator
        {
            bool operator()(const Ref<JobDeclaration>& _lhs, const Ref<JobDeclaration>& _rhs) const
            {
                return _lhs->m_Priority < _rhs->m_Priority;
            }
        };

        static int GetSystemThreads() { return s_JobSubsystem->m_SystemThreads; }
    
        static int GetActiveThreads() { return s_JobSubsystem->m_ActiveWorkerThreads; }
        static int& GetActiveThreadsRef() { return s_JobSubsystem->m_ActiveWorkerThreads; }
        static size_t GetJobQueueSize() { return s_JobSubsystem->m_Jobs.size(); }

        static std::vector<Ref<Thread>> GetWorkerThreads() { return s_JobSubsystem->m_WorkerThreads; }
        
        static Ref<JobSubsystem> Get() { return s_JobSubsystem->shared_from_this(); }

        std::vector<Ref<JobDeclaration>> JobProfileBuffer;
    private:
        /**
         * 
         * @return 
         */
        static Ref<JobDeclaration> RequestJob();

        void Initialize() override;

        void Deinitialize() override;

        /**
      * Thread Safe queue of jobs. Uses Custom JobComparator to sort jobs by priority
      * AddJob() pushes jobs to the queue. RequestJob() pops the top job from the queue. 
      * Microsoft Implementation of a concurrent_priority_queue https://learn.microsoft.com/en-us/cpp/parallel/concrt/reference/concurrent-priority-queue-class?view=msvc-170
      */
        Concurrency::concurrent_priority_queue<Ref<JobDeclaration>, JobComparator> m_Jobs;

        static JobSubsystem* s_JobSubsystem;

        std::vector<Ref<Thread>> m_WorkerThreads;

        
        /**
         * Number of threads available on the system
         */
        int m_SystemThreads;

        /**
         * Number of worker threads initialized
         */
        int m_AvailableWorkerThreads;

        /**
         * Number of worker threads currently active. Can be adjusted at runtime
         */
        int m_ActiveWorkerThreads;
        
        friend class Engine;
        friend class Thread;
    };
}
