#pragma once

#include "Profile.h"
#include "Denix/Core/Thread/JobProfile.h"

#include <unordered_map>
#include <vector>

namespace Denix
{
    struct JobDeclaration;

    /**
     * Struct to hold thread-specific profiling data.
     */
    struct ThreadData
    {
        size_t m_ThreadID = 0; ///< ID of the thread.
        size_t m_JobExecCount = 0; ///< Number of jobs executed by the thread.
        float m_ThreadExecTime = 0.0f; ///< Total execution time of the thread.
        float m_ThreadSleepTime = 0.0f; ///< Total sleep time of the thread.
    };

    /**
     * ProfileSession class for managing a profiling session.
     * Inherits from Object.
     */
    class ProfileSession final : public Object
    {
    public:
        explicit ProfileSession(const ObjectInit& _objInit): Object(_objInit),
                                                             m_MinFrameTime(0),
                                                             m_MaxFrameTime(0),
                                                             m_TotalFrameTime(0),
                                                             m_AverageFrameTime(0),
                                                             m_AverageFramesPerSecond(0), m_GraphHistory(10.0f),
                                                             m_IsProfiling(false)
        {
            // Reserve to reduce reallocations during profiling.
            m_InlineProfileMap.reserve(50);
            m_InlineProfiles.reserve(50);
            m_JobProfilesMap.reserve(50);
            m_JobProfiles.reserve(50);
            m_SessionTimer = MakeRef<Timer>();
        }

        ~ProfileSession() override = default;

        /**
         * Get the session timer.
         * @return Reference to the session timer.
         */
        Ref<Timer> GetSessionTimer() const { return m_SessionTimer; }

        /**
         * Check if profiling is active.
         * @return True if profiling is active, false otherwise.
         */
        bool IsProfiling() const { return m_IsProfiling; }

        /**
         * Get the map of inline profiles.
         * @return Reference to the map of inline profiles.
         */
        std::unordered_map<std::string, Ref<Profile>>& GetInlineProfileMap() { return m_InlineProfileMap; }

        /**
         * Get the list of inline profiles.
         * @return Reference to the vector of inline profiles.
         */
        std::vector<Ref<Profile>>& GetInlineProfiles() { return m_InlineProfiles; }

        /**
         * Get the map of job profiles.
         * @return Reference to the map of job profiles.
         */
        std::unordered_map<std::string, Ref<JobProfile>>& GetJobProfileMap() { return m_JobProfilesMap; }

        /**
         * Get the list of job profiles.
         * @return Reference to the vector of job profiles.
         */
        std::vector<Ref<JobProfile>>& GetJobProfiles() { return m_JobProfiles; }

        /**
         * Get the list of thread data.
         * @return Reference to the vector of thread data.
         */
        std::vector<ThreadData>& GetThreadData() { return m_ThreadData; }

        /**
         * Get the list of thread job counts.
         * @return Reference to the vector of thread job counts.
         */
        std::vector<int>& GetThreadJobCounts() { return m_ThreadJobCounts; }

        /**
         * Get the list of thread job execution times.
         * @return Reference to the vector of thread job execution times.
         */
        std::vector<float>& GetThreadJobExecTimes() { return m_ThreadJobExecTimes; }

        /**
         * Get the list of thread sleep times.
         * @return Reference to the vector of thread sleep times.
         */
        std::vector<float>& GetThreadSleepTimes() { return m_ThreadSleepTimes; }

        /**
         * Get the start time of the session.
         * @return Start time of the session.
         */
        float GetStartTime() const { return m_SessionTimer->GetStartTime(); }

        /**
         * Get the end time of the session.
         * @return End time of the session.
         */
        float GetEndTime() const { return m_SessionTimer->GetEndTime(); }

        /**
         * Get the duration of the session.
         * @return Duration of the session.
         */
        float GetDuration() const { return m_SessionTimer->GetDuration(); }

        /**
         * Get the average frames per second.
         * @return Average frames per second.
         */
        int GetAverageFramesPerSecond() const { return m_AverageFramesPerSecond; }

        /**
         * Get the average frame time.
         * @return Average frame time.
         */
        float GetAverageFrameTime() const { return m_AverageFrameTime; }

        /**
         * Get the average frame time in milliseconds.
         * @return Average frame time in milliseconds.
         */
        float GetAverageFrameTimeMs() const { return m_AverageFrameTime * 1000.0f; }

        /**
         * Get the minimum frame time.
         * @return Minimum frame time.
         */
        float GetMinFrameTime() const { return m_MinFrameTime; }

        /**
         * Get the minimum frame time in milliseconds.
         * @return Minimum frame time in milliseconds.
         */
        float GetMinFrameTimeMs() const { return m_MinFrameTime * 1000.0f; }

        /**
         * Get the maximum frame time.
         * @return Maximum frame time.
         */
        float GetMaxFrameTime() const { return m_MaxFrameTime; }

        /**
         * Get the maximum frame time in milliseconds.
         * @return Maximum frame time in milliseconds.
         */
        float GetMaxFrameTimeMs() const { return m_MaxFrameTime * 1000.0f; }

        /**
         * Get the graph history duration.
         * @return Graph history duration.
         */
        float& GetGraphHistory() { return m_GraphHistory; }

        /**
         * Get the graph history duration.
         * @return Graph history duration.
         */
        float GetGraphHistory() const { return m_GraphHistory; }

    private:
        /**
         * Start the profiling session.
         */
        void StartSession();

        /**
         * End the profiling session.
         */
        void EndSession();

        /**
         * Start an inline profile with the given name.
         * @param _name Name of the inline profile.
         */
        void StartInlineProfile(const std::string& _name);

        /**
         * End the inline profile with the given name.
         * @param _name Name of the inline profile.
         */
        void EndInlineProfile(const std::string& _name);

        /**
         * Start a job profile for the given job.
         * @param _job Reference to the job declaration.
         */
        void StartJobProfile(const Ref<JobDeclaration>& _job);

        /**
         * End the job profile for the given job.
         * @param _job Reference to the job declaration.
         */
        void EndJobProfile(const Ref<JobDeclaration>& _job);

        /**
         * Get the inline profile with the given name.
         * @param _name Name of the inline profile.
         * @return Reference to the inline profile.
         */
        Ref<Profile> GetInlineProfile(const std::string& _name);

        /**
         * Get the job profile with the given name.
         * @param _name Name of the job profile.
         * @return Reference to the job profile.
         */
        Ref<JobProfile> GetJobProfile(const std::string& _name);

        std::unordered_map<std::string, Ref<Profile>> m_InlineProfileMap; ///< Map of inline profiles.
        std::vector<Ref<Profile>> m_InlineProfiles; ///< List of inline profiles.

        std::unordered_map<std::string, Ref<JobProfile>> m_JobProfilesMap; ///< Map of job profiles.
        std::vector<Ref<JobProfile>> m_JobProfiles; ///< List of job profiles.

        std::vector<ThreadData> m_ThreadData; ///< List of thread data.
        std::vector<int> m_ThreadJobCounts; ///< List of thread job counts.
        std::vector<float> m_ThreadJobExecTimes; ///< List of thread job execution times.
        std::vector<float> m_ThreadSleepTimes; ///< List of thread sleep times.

        Ref<Timer> m_SessionTimer; ///< Timer for the session.

        std::vector<float> m_FrameTimes; ///< List of frame times.
        std::vector<int> m_FramesPerSeconds; ///< List of frames per second.

        float m_MinFrameTime; ///< Minimum frame time.
        float m_MaxFrameTime; ///< Maximum frame time.
        float m_TotalFrameTime; ///< Total frame time.
        float m_AverageFrameTime; ///< Average frame time.
        int m_AverageFramesPerSecond; ///< Average frames per second.

        float m_GraphHistory; ///< Graph history duration.
        bool m_IsProfiling; ///< Flag indicating if profiling is active.

        friend class ProfileSubsystem;
        friend class JobSubsystem;
        friend class TimerSubsystem;
    };
}
