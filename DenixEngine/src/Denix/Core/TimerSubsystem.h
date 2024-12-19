#pragma once

#include "Denix/System/Subsystem.h"
#include "Denix/Core.h"
#include "Timer.h"

namespace Denix
{
	class Profile;

	class TimerSubsystem final : public Subsystem
	{
	public:
		TimerSubsystem();
		~TimerSubsystem() override;

		TimerSubsystem(const TimerSubsystem& _other) = delete;
		TimerSubsystem(TimerSubsystem&& _other) noexcept = delete;
		TimerSubsystem& operator=(const TimerSubsystem& _other) = delete;
		TimerSubsystem& operator=(TimerSubsystem&& _other) noexcept = delete;
		
		static TimerSubsystem* Get() { return s_TimerSubsystem; }

		void Initialize() override;

		void Deinitialize() override;

		void BeginFrame();
		void EndFrame();

		static float GetProgramElaspedTime();
		static int GetFPS();
		static int& GetMaxFPS() { return s_TimerSubsystem->m_MaxLimitFPS; }
		static float GetFrameTime();
		static float GetFrameTimeMs();
		static float GetFrameTimeMsAverage();
		
		static float GetDeltaTime() {return s_TimerSubsystem->m_DeltaTime; }
		static float& GetGameTimeSpeed() { return s_TimerSubsystem->m_GameTimeSpeed; }

		Ref<Profile> m_EngineProfile;

	private:
		static TimerSubsystem* s_TimerSubsystem;

		int m_FramesPerSecond;

		/**
		 * Time taken for frame to complete in milliseconds
		 */
		float m_FrameTime;
		
		/**
		 * Game specifc frame time
		 */
		float m_DeltaTime;
		
		/**
		 * 
		 */
		float m_GameTimeSpeed;

		
		/**
		 * 
		 */
		int m_MaxLimitFPS;

		
		std::vector<Ref<Timer>> m_Timers;
		
		friend class Engine;
		friend class ProfileSubsystem;
	};
}
