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
		
		static int GetFPS();
		static int& GetMaxFPS() { return s_TimerSubsystem->m_MaxFPS; }
		static float GetFrameTime();
		static float GetFrameTimeMs();

		static float GetDeltaTime() {return s_TimerSubsystem->m_DeltaTime; }
		static float& GetGameTimeSpeed() { return s_TimerSubsystem->m_GameTimeSpeed; }

		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
		Ref<Profile> m_EngineProfile;

	private:
		static TimerSubsystem* s_TimerSubsystem;

		int m_FramesPerSecond;

		/**
		 * Time taken for frame to complete in milliseconds
		 */
		float m_FrameTime;
		
		/**
		 * Time taken for frame to complete in seconds
		 */
		float m_FrameTimeMs;

		/**
		 * Game specifc frame time
		 */
		float m_DeltaTime;
		
		/**
		 * 
		 */
		float m_GameTimeSpeed;

		
		/**
		 * Maximum frames per second
		 */
		int m_MaxFPS;

		
		std::vector<Ref<Timer>> m_Timers;
		
		friend class Engine;
		friend class ProfileSubsystem;
	};
}
