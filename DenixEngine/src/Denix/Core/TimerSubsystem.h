#pragma once

#include "Denix/System/Subsystem.h"
#include "Denix/Core.h"
#include "Timer.h"

namespace Denix
{
	class TimerSubsystem : public Subsystem
	{
	public:
		TimerSubsystem();

		~TimerSubsystem();

		static TimerSubsystem* Get() { return s_TimerSubsystem; }

		void Initialize();

		void Deinitialize();

		void BeginFrame();

		void EndFrame();

		template<typename Period = std::chrono::seconds>
		static float GetProgramElapsedTime()
		{
			return s_TimerSubsystem->m_FrameTimer->GetElapsed<Period>();
		}
		
		static int GetFPS();
		static int& GetMaxFPS() { return s_TimerSubsystem->m_MaxFPS; }
		static float GetFrameTime();
		static float GetFrameTimeMs();
		static float& GetGameTimeSpeed() { return s_TimerSubsystem->m_GameTimeSpeed; }

		Ref<Timer> m_FrameTimer;
	private:
		static TimerSubsystem* s_TimerSubsystem;

		std::chrono::time_point<std::chrono::system_clock> start, end;

		int m_FramesPerSecond;
		float m_FrameTimeS;
		float m_FrameTimeMs;
		float m_DeltaTime;
		float m_GameTimeSpeed;

		
		/**
		 * Maximum frames per second
		 */
		int m_MaxFPS;

		
		std::vector<Ref<Timer>> m_Timers;
		
		friend class Engine;
	};
}
