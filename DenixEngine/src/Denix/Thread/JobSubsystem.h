#pragma once
#include "Denix/System/Subsystem.h"

#include "Denix/Thread/Thread.h"

#include "Denix/Profile/ProfileSubsystem.h"

#include <concurrent_priority_queue.h>

namespace Denix
{
    /**
         * \brief Comparator for job priority.
         */
    struct JobComparator
    {
        /**
             * \brief Compares two jobs based on their priority.
             * \param _lhs The left-hand side job.
             * \param _rhs The right-hand side job.
             * \return True if the left-hand side job has lower priority than the right-hand side job.
             */
        bool operator()(const Ref<JobDeclaration>& _lhs, const Ref<JobDeclaration>& _rhs) const
        {
            return _lhs->m_Priority < _rhs->m_Priority;
        }
    };
    
    /**
        * \class JobSubsystem
        * \brief Manages job scheduling and execution within the system.
        *
        * The JobSubsystem is responsible for managing and executing jobs across multiple threads.
        * It provides functionality to add jobs, start and stop thread profiling, and manage worker threads.
        */
    class JobSubsystem : public Subsystem, public std::enable_shared_from_this<JobSubsystem>
    {
    public:
        /**
         * \brief Constructs the JobSubsystem.
         */
        JobSubsystem()
        {
            s_JobSubsystem = this;
            m_SystemThreads = 0;
            m_ActiveWorkerThreads = 0;
            m_AvailableWorkerThreads = 0;
            
            DE_LOG_CREATE(LogThread)
            DE_LOG_CREATE(LogJob)
        }

        /**
        * \brief Destructs the JobSubsystem.
        */
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
        * \brief Checks if thread profiling is enabled.
        * \return True if profiling is enabled, false otherwise.
        */
        static bool IsProfiling() { return Thread::s_ShouldProfile; }
        
        /**
        * \brief Adds a job to the job queue.
        * \tparam Func The type of the function to execute.
        * \tparam Args The types of the arguments to pass to the function.
        * \param _name The name of the job.
        * \param _priority The priority of the job.
        * \param _waitCounter The counter to wait on before executing the job.
        * \param _func The function to execute.
        * \param _args The arguments to pass to the function.
        */
        template <typename Func, typename... Args>
        static void AddJob(const std::string& _name, const Priority _priority, const Ref<Counter>& _waitCounter,
                           Func&& _func, Args&&... _args)
        {
            Ref<JobDeclaration> job = MakeRef<JobDeclaration>(
                _name,
                _priority,
                _waitCounter ? _waitCounter : MakeRef<Counter>(1),
                std::bind(std::forward<Func>(_func), std::forward<Args>(_args)...)
                );

            s_JobSubsystem->m_Jobs.push(job);
        }

        /**
         * \brief Adds and executes a job inline on the main thread.
         *
         *  This is useful for executing thread specific tasks that should not be offloaded to the worker threads.
         *  Job information is still proccessed by the JobSubsystem for profiling and tracking purposes.
         * \tparam Func The type of the function to execute.
         * \tparam Args The types of the arguments to pass to the function.
         * \param _name The name of the job.
         * \param _priority The priority of the job.
         * \param _waitCounter The counter to wait on before executing the job.
         * \param _func The function to execute.
         * \param _args The arguments to pass to the function.
         */
        template <typename Func, typename... Args>
        static void AddJobInline(const std::string& _name, const Priority _priority, const Ref<Counter>& _waitCounter,
                           Func&& _func, Args&&... _args)
        {
            Ref<JobDeclaration> job = MakeRef<JobDeclaration>(
                _name,
                _priority,
                _waitCounter ? _waitCounter : MakeRef<Counter>(1),
                std::bind(std::forward<Func>(_func), std::forward<Args>(_args)...)
                );

            // Hardcoded to the main thread
            job->m_ThreadIndex = 0;

            // Execute the job
            if (Thread::s_ShouldProfile) DE_PROFILE_JOB(job)
            job->m_EntryPoint();
            if (Thread::s_ShouldProfile) DE_PROFILE_JOB_END(job)
            job->m_WaitCounter->Decrement();
        }

        /**
        * \brief Starts profiling for all threads.
        */
        static void StartThreadProfiling()
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

        /**
         * \brief Stops profiling for all threads.
         */
        static void StopThreadProfiling()
        {
            Thread::s_ShouldProfile = false;
            DE_LOG(LogThread, Info, "Thread Profiling: Disabled")
        }

        /**
       * \brief Updates the number of active worker threads based on m_ActiveWorkerThreads
       * 
       * Active worker threads is clamped to the available worker threads.
       * Threads will complete their current job before being deactivated.
       */
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
        
        
        /**
        * \brief Gets the number of system threads.
        * \return The number of system threads.
        */
        static int GetSystemThreads() { return s_JobSubsystem->m_SystemThreads; }

        /**
        * \brief Gets the number of active worker threads.
        * \return The number of active worker threads.
        */
        static int GetActiveThreads() { return s_JobSubsystem->m_ActiveWorkerThreads; }

        /**
         * \brief Gets a reference to the number of active worker threads.
         *
         * Adjusting this value will not effect the number of worker threads until UpdateActiveThreads() is called.
         * \return A reference to the number of active worker threads.
         */
        static int& GetActiveThreadsRef() { return s_JobSubsystem->m_ActiveWorkerThreads; }

        /**
         * \brief Gets the size of the job queue.
         *
         * Usfeul for debugging and profiling purposes at different synchronization points.
         * \return The size of the job queue.
         */
        static size_t GetJobQueueSize() { return s_JobSubsystem->m_Jobs.size(); }

        /**
         * \brief Gets the worker threads.
         * \return A vector of references to the worker threads.
         */
        static std::vector<Ref<Thread>>& GetWorkerThreads() { return s_JobSubsystem->m_WorkerThreads; }

        /**
         * \brief Gets a shared reference to the JobSubsystem.
         * \return A shared reference to the JobSubsystem.
         */
        static Ref<JobSubsystem> Get() { return s_JobSubsystem->shared_from_this(); }

    private:
    /**
     * \brief Requests the next job from the job queue.
     * 
     * This function pops the top job from the concurrent priority queue of jobs.
     * The job with the highest priority will be returned.
     * 
     * \return A reference to the next job declaration.
     */
    static Ref<JobDeclaration> RequestJob();

        /**
         * \brief Initializes the JobSubsystem.
         *
         * Establishes the number of system threads and initializes the worker threads.
         */
        void Initialize() override;

        /**
         * \brief Deinitializes the JobSubsystem.
         *
         * Join all worker threads and clear the job queue.
         */
        void Deinitialize() override;

        /**
      * \brief Thread-safe queue of jobs, sorted by priority. JobComparator struct is used to compare job priorities.
      * AddJob() pushes jobs to the queue. RequestJob() pops the top job from the queue. 
      * Microsoft Implementation of a concurrent_priority_queue https://learn.microsoft.com/en-us/cpp/parallel/concrt/reference/concurrent-priority-queue-class?view=msvc-170
      */
        Concurrency::concurrent_priority_queue<Ref<JobDeclaration>, JobComparator> m_Jobs;

        /* Static JobSubsystem instance */
        static JobSubsystem* s_JobSubsystem; 

        std::vector<Ref<Thread>> m_WorkerThreads;
        
        /**
         * Number of threads available on the system
         */
        int m_SystemThreads;

        /**
         * \brief Number of worker threads initialized
         *
         * This Value is always n -1 where n is the number of system threads.
         * This ensures our process only uses the available system threads to reduce contention.
         */
        int m_AvailableWorkerThreads;

        /**
         * \brief Number of active worker threads.
         *
         * Adjusting this value will not effect the number of worker threads until UpdateActiveThreads() is called.
         */
        int m_ActiveWorkerThreads;
        
        friend class Engine;
        friend class Thread;
    };
}
