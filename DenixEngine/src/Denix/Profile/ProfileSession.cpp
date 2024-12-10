#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "ProfileSession.h"

void Denix::ProfileSession::StartProfile(const std::string& _name)
{
    Profile& profile = GetProfile(_name);
    profile.Start();
}

void Denix::ProfileSession::EndProfile(const std::string& _name)
{
    Profile& profile = GetProfile(_name);
    profile.End();
}

Denix::Profile& Denix::ProfileSession::GetProfile(const std::string& _name)
{
    // Create profile if it doesn't exist. Occurs on first instance of profile
    if (!m_Profiles.contains(_name))
    {
        m_Profiles[_name] = Profile({_name});
        DE_LOG(LogProfile, Trace, "Profile {} created", _name)
    }
    
    return m_Profiles[_name];
}