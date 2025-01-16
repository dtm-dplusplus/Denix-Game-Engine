#pragma once
#include "Denix/System/Subsystem.h"

#include "Denix/Thread/Thread.h"

#include "Denix/Profile/ProfileSubsystem.h"

#include <concurrent_priority_queue.h>
//#include "concurrentqueue.h"

namespace Denix
{
    /**
         * \brief Comparator for job priority.
         */
    struct JobComparator//: public moodycamel::ConcurrentQueueDefaultTraits
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
    class JobSubsystem final: public Subsystem<JobSubsystem>
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
        * \tparam Func The function to execute.
        * \tparam Args The types of the arguments to pass to the function.
        * \param _name The name of the job.
        * \param _priority The priority of the job.
        * \param _waitCounter The counter used to manage related job dependencies.
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

            //DE_LOG(LogJob, Trace, "Creating Job: {}", _name)
            // Create the job
            Ref<JobDeclaration> job = MakeRef<JobDeclaration>(
                _name,
                _priority,
                _waitCounter,
                std::bind(std::forward<Func>(_func), std::forward<Args>(_args)...)
            );

            //LockGuard lockGuard(*s_Instance->m_JobMutex);
            std::lock_guard lock(s_Instance->m_STDMutex);
            
           //DE_LOG(LogJob, Trace, "Adding Job to Queue: {}", _name)
            //s_Instance->m_Jobs.try_enqueue(job);
            s_Instance->m_Jobs.push(job);
            //DE_LOG(LogJob, Trace, "Job Added to Queue: {}", _name)
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

            // Execute the job with thread logic
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
                                const Ref<Counter>& _waitCounter, std::vector<T>& _objects, Func&& _func,
                                Args&&... _args)
        {
            // Check the vector size
            const size_t objectCount = _objects.size();
            if (objectCount == 0) return;

            if (objectCount > s_Instance->m_BatchUpdateThreshold)
            {
                // Setup batch size. All batches are equal with the exception of the last batch.
                const size_t batchSize = (objectCount + s_Instance->m_ActiveWorkerThreads - 1) / s_Instance->m_ActiveWorkerThreads;

                // Create a job for each batch
                for (size_t batchIndex = 0; batchIndex < s_Instance->m_ActiveWorkerThreads; ++batchIndex)
                {
                    size_t begin = batchIndex * batchSize;
                    size_t end = std::min(begin + batchSize, objectCount);

                    if (begin >= objectCount) break;

                    AddJob(_namePrefix + std::to_string(batchIndex), _priority, _waitCounter,
                           [begin, end, &_objects, _func, _args...] {
                               for (size_t j = begin; j < end; ++j) {
                                   std::invoke(_func, _objects[j], _args...);
                               }
                           });
                }
            }
            else
            {
                // Create a single job for the entire vector to avoid overhead
                AddJob(_namePrefix + " Below Threshold", _priority, _waitCounter, [&_objects, _func, _args...]
                {
                    for (auto& obj : _objects) std::invoke(_func, obj, _args...);
                });
            }
            DE_PROFILE_END(AddJobBatch)
        }

        /**
       * \brief Updates the number of active worker threads based on m_ActiveWorkerThreads
       * 
       * Active worker threads is clamped to the available worker threads.
       * Threads will complete their current job before being deactivated.
       */
        static void UpdateActiveThreads();


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

        /**
         * \brief Gets the number of available worker threads.
         * Do not confuse this with the number of active worker threads.
         * @return The number of available worker threads. 
         */
        static int GetAvailableThreads() { return s_Instance->m_AvailableWorkerThreads; }
        
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
         * It's safe to assume all jobs have been completed at this point.
         */
        void Deinitialize() override;

        /**
      * \brief Thread-safe queue of jobs, sorted by priority. JobComparator struct is used to compare job priorities.
      * AddJob() pushes jobs to the queue. RequestJob() pops the top job from the queue. 
      * Microsoft Implementation of a concurrent_priority_queue https://learn.microsoft.com/en-us/cpp/parallel/concrt/reference/concurrent-priority-queue-class?view=msvc-170
      */
        Concurrency::concurrent_priority_queue<Ref<JobDeclaration>, JobComparator> m_Jobs;

        Ref<Mutex> m_JobMutex;

        std::mutex m_RequestJobMutex;
        std::mutex  m_STDMutex;
        
        std::vector<Ref<Thread>> m_WorkerThreads;

        /**
         * \brief The threshold for batch updates.
         * If number of objects is above this threshold, the job will be split into batches.
         */
        int m_BatchUpdateThreshold;

        /**
         * Number of threads available on the system
         */
        int m_SystemThreads;

        /**
         * \brief Number of worker threads initialized
         *
         * This Value is always n -1 to account for main thread where n is the number of system threads.
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
