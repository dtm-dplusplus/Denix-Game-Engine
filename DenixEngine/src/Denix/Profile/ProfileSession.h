#pragma once
#include "Profile.h"

namespace Denix
{
    class ProfileSession: public Object
    {
    public:
        ProfileSession(const ObjectInit& _objInit): Object(_objInit)
        {
            // Reserve to reduce reallocations during profiling.
            m_Profiles.reserve(25);
        }
        ~ProfileSession() override = default;

        std::unordered_map<std::string, Profile>& GetProfiles() { return m_Profiles; }
    private:
        void StartProfile(const std::string& _name);
        void EndProfile(const std::string& _name);

        Profile& GetProfile(const std::string& _name);
        std::unordered_map<std::string, Profile> m_Profiles;

        friend class ProfileSubsystem;
    };
}