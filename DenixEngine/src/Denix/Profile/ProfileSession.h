#pragma once

#include "Profile.h"
#include "Denix/Thread/JobProfile.h"

#include <unordered_map>
#include <vector>

namespace Denix
{
    struct JobDeclaration;

    struct ThreadData
    {
        size_t m_ThreadID = 0;
        size_t m_JobExecCount = 0;
        float m_ThreadExecTime = 0.0f;
        float m_ThreadSleepTime = 0.0f;
    };

    class ProfileSession final : public Object
    {
    public:
        explicit ProfileSession(const ObjectInit& _objInit): Object(_objInit),
                                                            m_MinFrameTime(0),
                                                             m_MaxFrameTime(0),
                                                             m_TotalFrameTime(0),
                                                             m_AverageFrameTime(0),
                                                             m_AverageFramesPerSecond(0),
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

        Ref<Timer> GetSessionTimer() const { return m_SessionTimer; }

        bool IsProfiling() const { return m_IsProfiling; }

        std::unordered_map<std::string, Ref<Profile>>& GetInlineProfileMap() { return m_InlineProfileMap; }
        std::vector<Ref<Profile>>& GetInlineProfiles() { return m_InlineProfiles; }

        std::unordered_map<std::string, Ref<JobProfile>>& GetJobProfileMap() { return m_JobProfilesMap; }
        std::vector<Ref<JobProfile>>& GetJobProfiles() { return m_JobProfiles; }

        std::vector<ThreadData>& GetThreadData() { return m_ThreadData; }
        std::vector<int>& GetThreadJobCounts() { return m_ThreadJobCounts; }
        std::vector<float>& GetThreadJobExecTimes() { return m_ThreadJobExecTimes; }
        std::vector<float>& GetThreadSleepTimes() { return m_ThreadSleepTimes; }

        int GetAverageFramesPerSecond() const { return m_AverageFramesPerSecond; }
        float GetAverageFrameTime() const { return m_AverageFrameTime; }
        float GetMinFrameTime() const { return m_MinFrameTime; }
        float GetMaxFrameTime() const { return m_MaxFrameTime; }

    private:
        void StartSession();
        void EndSession();
        void StartInlineProfile(const std::string& _name);
        void EndInlineProfile(const std::string& _name);

        void StartJobProfile(const Ref<JobDeclaration>& _job);
        void EndJobProfile(const Ref<JobDeclaration>& _job);

        Ref<Profile> GetInlineProfile(const std::string& _name);
        Ref<JobProfile> GetJobProfile(const std::string& _name);

        std::unordered_map<std::string, Ref<Profile>> m_InlineProfileMap;
        std::vector<Ref<Profile>> m_InlineProfiles;

        std::unordered_map<std::string, Ref<JobProfile>> m_JobProfilesMap;
        std::vector<Ref<JobProfile>> m_JobProfiles;

        std::vector<ThreadData> m_ThreadData;
        std::vector<int> m_ThreadJobCounts;
        std::vector<float> m_ThreadJobExecTimes;
        std::vector<float> m_ThreadSleepTimes;

        Ref<Timer> m_SessionTimer;

        std::vector<float> m_FrameTimes;
        std::vector<int> m_FramesPerSeconds;

        float m_MinFrameTime;
        float m_MaxFrameTime;
        float m_TotalFrameTime;
        float m_AverageFrameTime;
        int m_AverageFramesPerSecond;

        bool m_IsProfiling;

        friend class ProfileSubsystem;
        friend class JobSubsystem;
        friend class TimerSubsystem;
    };
}
