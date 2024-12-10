#pragma once

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"
#include "Profile.h"
#include "ProfileSession.h"

#define DE_PROFILE(name) ProfileSubsystem::StartProfile(#name);
#define DE_PROFILE_END(name) ProfileSubsystem::EndProfile(#name);

namespace Denix
{
	class ProfileSubsystem final : public Subsystem
	{
	public:
		ProfileSubsystem();

		~ProfileSubsystem() override;

		ProfileSubsystem(const ProfileSubsystem& _other) = delete;
		ProfileSubsystem(ProfileSubsystem&& _other) noexcept = delete;
		ProfileSubsystem& operator=(const ProfileSubsystem& _other) = delete;
		ProfileSubsystem& operator=(ProfileSubsystem&& _other) noexcept = delete;
		
		static ProfileSubsystem* Get() { return s_ProfileSubsystem; }

		void Initialize() override;

		void Deinitialize() override;

		static void StartProfileSession();
		static void EndProfileSession();
		
		static void StartProfile(const std::string& _name);
		static void EndProfile(const std::string& _name);

		static Ref<ProfileSession> GetActiveProfileSession() { return s_ProfileSubsystem->m_ActiveProfileSession; }

		Ref<ProfileSession> m_ActiveProfileSession;

		std::vector<Ref<ProfileSession>> m_ProfileSessions;
	private:
		static ProfileSubsystem* s_ProfileSubsystem;

		std::map<std::string, Ref<Profile>> m_Profiles;
		
		friend class Engine;
	};
}
