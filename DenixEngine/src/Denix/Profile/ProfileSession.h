#pragma once
#include "Profile.h"
#include "Denix/Thread/JobProfile.h"

namespace Denix
{
    struct JobDeclaration;

    class ProfileSession: public Object
    {
    public:
        ProfileSession(const ObjectInit& _objInit): Object(_objInit)
        {
            // Reserve to reduce reallocations during profiling.
            m_InlineProfiles.reserve(25);
        }
        ~ProfileSession() override = default;

        std::unordered_map<std::string, Profile>& GetProfiles() { return m_InlineProfiles; }
        std::unordered_map<std::string, JobProfile>& GetJobProfiles() { return m_JobProfiles; }
        
    private:
        void StartProfile(const std::string& _name);
        void EndProfile(const std::string& _name);

        void StartJobProfile(const Ref<JobDeclaration>& _job);
        void EndJobProfile(const Ref<JobDeclaration>& _job);
        
        Profile& GetProfile(const std::string& _name);
        JobProfile& GetJobProfile(const std::string& _name);
        std::unordered_map<std::string, Profile> m_InlineProfiles;
        
        std::unordered_map<std::string, JobProfile> m_JobProfiles;

        friend class ProfileSubsystem;
    };


}
