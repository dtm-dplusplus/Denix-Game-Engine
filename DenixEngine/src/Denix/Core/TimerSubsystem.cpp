#include "TimerSubsystem.h"

#include "Denix/Profile/Profile.h"
#include "Denix/Profile/ProfileSubsystem.h"

namespace Denix
{
	TimerSubsystem* TimerSubsystem::s_TimerSubsystem{ nullptr };

	TimerSubsystem::TimerSubsystem()
	{
		s_TimerSubsystem = this;
		DE_LOG_CREATE(LogTimer)

		m_FrameTime = 0.167f;
		m_DeltaTime = 0.167f;
		m_FramesPerSecond = 60;
		m_GameTimeSpeed = 1.0f;
		m_MaxLimitFPS = 60;
	}

	TimerSubsystem::~TimerSubsystem()
	{
		s_TimerSubsystem = nullptr;
	}

	void TimerSubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogTimer, Warn, "Initializing TimerSubsystem")
		Timer::m_ProgramStartTimePoint = std::chrono::high_resolution_clock::now();
		m_EngineProfile = MakeRef<Profile>(ObjectInit("EngineProfile"));
		DE_LOG(LogTimer, Info, "TimerSubsystem Initialized")
	}

	void TimerSubsystem::Deinitialize()
	{
		DE_LOG(LogTimer, Trace, "TimerSubsystem Deinitialized")
	}

	void TimerSubsystem::BeginFrame()
	{
		m_EngineProfile->Start();
	}

	void TimerSubsystem::EndFrame()
	{
		// Calculate the time taken for the frame to complete
		const float frameTimeMs = m_EngineProfile->m_Timer->GetElapsedMs();

		// Implement max frame rate
		// We Should potentially skip draw calls instead of waiting here.
		// need to check if vsync is enabled.
		if (const float minFrameTime = 1000.0f / static_cast<float>(m_MaxLimitFPS);
			frameTimeMs < minFrameTime && m_MaxLimitFPS > 0)
		{
			const float sleepTime = minFrameTime - frameTimeMs;
			Timer waitTimer;
			waitTimer.Start();
			while (waitTimer.GetElapsedMs() < sleepTime){}
		}

		// Calculate the real time taken for the frame to complete
		m_EngineProfile->End();
		m_FrameTime = m_EngineProfile->GetLastProfileDuration();

		// Calculate the delta time, accounting for the game time speed which can be used to slow down or speed up the game.
		m_DeltaTime = m_GameTimeSpeed * m_FrameTime;
		
		static int frameCounter = 0;
		frameCounter++;

		static float timeInFrame = 0.0f;
		timeInFrame += m_FrameTime;
		
		// Calculate the frames per second. When a second has passed, reset the frame counter and update the frames per second.
		if (timeInFrame >= 1.0f)
		{
			m_FramesPerSecond = frameCounter;
			frameCounter = 0;
			timeInFrame = 0.0f;
		}
	}

	float TimerSubsystem::GetProgramElaspedTime()
	{
		return Timer::GetProgramElaspedTime();
	}

	int TimerSubsystem::GetFPS()
	{
		return s_TimerSubsystem->m_FramesPerSecond;
	}

	float TimerSubsystem::GetFrameTime() { return s_TimerSubsystem->m_FrameTime; }
	float TimerSubsystem::GetFrameTimeMs() {return s_TimerSubsystem->m_FrameTime * 1000.0f; }

	float TimerSubsystem::GetFrameTimeMsAverage()
	{ return s_TimerSubsystem->m_EngineProfile->m_AverageDuration * 1000.0f; }
}
