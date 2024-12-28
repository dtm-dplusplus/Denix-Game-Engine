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
    class JobSubsystem : public Subsystem<JobSubsystem>
    {
    public:
        /**
         * \brief Constructs the JobSubsystem.
         */
        JobSubsystem();

        /**
        * \brief Destructs the JobSubsystem.
        */
        ~JobSubsystem() override = default;

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
            // Validate Wait Counter
            if (!_waitCounter)
            {
                DE_LOG(LogJob, Error, "Wait Counter is null. Job not added: {}", _name)
                return;
            }

            // Increment the wait counter
            _waitCounter->Increment();

            // Create the job
            Ref<JobDeclaration> job = MakeRef<JobDeclaration>(
                _name,
                _priority,
                _waitCounter,
                std::bind(std::forward<Func>(_func), std::forward<Args>(_args)...)
            );

            s_Instance->m_Jobs.push(job);
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
            // Validate Wait Counter
            if (!_waitCounter)
            {
                DE_LOG(LogJob, Error, "Wait Counter is null. Job not added: {}", _name)
                return;
            }

            // Increment the wait counter
            _waitCounter->Increment();

            Ref<JobDeclaration> job = MakeRef<JobDeclaration>(
                _name,
                _priority,
                _waitCounter,
                std::bind(std::forward<Func>(_func), std::forward<Args>(_args)...)
            );

            // Hardcoded to the main thread
            job->m_ThreadIndex = 0;

            // Execute the job
            if (Thread::s_ShouldProfile)
                DE_PROFILE_JOB(job)
            job->m_EntryPoint();
            if (Thread::s_ShouldProfile)
                DE_PROFILE_JOB_END(job)
            job->m_WaitCounter->Decrement();
        }


        /**
         * \brief Parallel For Each function.
         *
         * This function divides the collection of objects into batches and creates a job for each batch.
         * Each job will execute the provided function on a subset of the objects.
         * If the batch size is below the batch threshold, a single job will be created.
         *
         * \tparam T The type of the objects in the collection.
         * \tparam Func The type of the function to execute.
         * \tparam Args The types of the arguments to pass to the function.
         * \param _namePrefix The prefix for the job names.
         * \param _priority The priority of the jobs.
         * \param _waitCounter The counter to wait on before executing the jobs.
         * \param _objects The collection of objects to process.
         * \param _func The function to execute on each object.
         * \param _args The arguments to pass to the function.
         */
        template <typename T, typename Func, typename... Args>
        static void AddJobBatch(const std::string& _namePrefix, const Priority _priority,
                              const Ref<Counter>& _waitCounter, std::vector<T>& _objects, Func&& _func, Args&&... _args)
        {
            const size_t objectCount = _objects.size();
            if (objectCount == 0)
            {
             //   DE_LOG(LogJob, Error, "Vector is empty. No jobs added: {}", _namePrefix)
                return;
            }

            // Prepare batch sizes

            // count = 20
            // bMod = 20/15 = 5 Remainder
            // bMax = 20/1 = 1
            // bSize = 20/15 = 1

            // count = 34
            // bMod = 34/15 = 4 Remainder
            // bSize = 34/15 = 2
            // bMax = 34/17 = 

            // count = 144
            // bMod = 144/15 = 9 Remainder
            // bSize = 144/15 = 9
            const size_t batchSize = objectCount / s_Instance->m_ActiveWorkerThreads;

            s_Instance->m_CurrentBatchCount = batchSize;

            if (objectCount > s_Instance->m_BatchUpdateThreshold && objectCount > s_Instance->
                m_AvailableWorkerThreads)
            {
                for (size_t batchIndex = 0; batchIndex < s_Instance->m_ActiveWorkerThreads; batchIndex++)
                {
                    size_t begin = batchIndex * batchSize;
                    size_t end = begin + batchSize;

                    if (batchIndex + 1 == s_Instance->m_AvailableWorkerThreads) end = objectCount;

                    AddJob(_namePrefix + std::to_string(batchIndex), _priority, _waitCounter,
                           [begin, end, &_objects, _func, _args...]
                           {
                               for (size_t j = begin; j < end; ++j) std::invoke(_func, _objects[j], _args...);
                           });
                }
            }
            else
            {
                AddJob(_namePrefix + " Below Threshold", _priority, _waitCounter, [&_objects, _func, _args...]
                {
                    for (auto& obj : _objects) std::invoke(_func, obj, _args...);
                });
            }
        }

        /**
       * \brief Updates the number of active worker threads based on m_ActiveWorkerThreads
       * 
       * Active worker threads is clamped to the available worker threads.
       * Threads will complete their current job before being deactivated.
       */
        static void UpdateActiveThreads();

        static bool& IsAutoBatchingEnabled() { return s_Instance->m_AutoBatchingEnabled; }

        static int& GetBatchUpdateThreshold() { return s_Instance->m_BatchUpdateThreshold; }

        /**
        * \brief Gets the number of system threads.
        * \return The number of system threads.
        */
        static int GetSystemThreads() { return s_Instance->m_SystemThreads; }

        /**
        * \brief Gets the number of active worker threads.
        * \return The number of active worker threads.
        */
        static int GetActiveThreads() { return s_Instance->m_ActiveWorkerThreads; }

        /**
         * \brief Gets a reference to the number of active worker threads.
         *
         * Adjusting this value will not effect the number of worker threads until UpdateActiveThreads() is called.
         * \return A reference to the number of active worker threads.
         */
        static int& GetActiveThreadsRef() { return s_Instance->m_ActiveWorkerThreads; }

        static size_t GetBatchSize() { return s_Instance->m_CurrentBatchCount; }

        /**
         * \brief Gets the size of the job queue.
         *
         * Usfeul for debugging and profiling purposes at different synchronization points.
         * \return The size of the job queue.
         */
        static size_t GetJobQueueSize() { return s_Instance->m_Jobs.size(); }

        /**
         * \brief Gets the worker threads.
         * \return A vector of references to the worker threads.
         */
        static std::vector<Ref<Thread>>& GetWorkerThreads() { return s_Instance->m_WorkerThreads; }

        /**
         * \brief Gets a shared reference to the JobSubsystem.
         * \return A shared reference to the JobSubsystem.
         */
        static Ref<JobSubsystem> Get() { return s_Instance->shared_from_this(); }

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
        * \brief Starts profiling for all threads.
        */
        static void StartThreadProfiling();

        /**
         * \brief Stops profiling for all threads.
         */
        static void StopThreadProfiling();

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

        std::vector<Ref<Thread>> m_WorkerThreads;

        size_t m_ManualBatchSize;
        int m_BatchUpdateThreshold;
        size_t m_BatchSizeMin;
        size_t m_CurrentBatchCount;
        bool m_AutoBatchingEnabled;

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
        friend class ProfileSubsystem;
        friend class Editor;
        friend class Thread;
    };
}
