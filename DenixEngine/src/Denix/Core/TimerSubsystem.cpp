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

		m_FrameTime = 0.33f;
		m_DeltaTime = 0.33f;
		m_FramesPerSecond = 30;
		m_GameTimeSpeed = 1.0f;
		m_MaxFPS = 60;
	}

	TimerSubsystem::~TimerSubsystem()
	{
		s_TimerSubsystem = nullptr;
	}

	void TimerSubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogTimer, Warn, "Initializing TimerSubsystem")
		m_FrameTimer = MakeRef<Timer>(ObjectInit("FrameTimer"), true);
		m_EngineProfile = MakeRef<Profile>(ObjectInit("EngineProfile"));
		m_EngineProfile->m_Visualize = true;
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
		m_EngineProfile->End();
		
		m_FrameTime = m_EngineProfile->GetDuration();
		m_FrameTimeS = m_FrameTime * 0.001f;

		// Calculate the delta time, accounting for the game time speed which can be used to slow down or speed up the game.
		m_DeltaTime = m_GameTimeSpeed * m_FrameTimeS;

		static int frameCounter = 0;
		frameCounter++;

		static float timeInFrame = 0.0f;
		timeInFrame += m_FrameTimeS;

		// Calculate the frames per second. When a second has passed, reset the frame counter and update the frames per second.
		if (timeInFrame >= 1.0f)
		{
			m_FramesPerSecond = frameCounter;
			frameCounter = 0;
			timeInFrame = 0.0f;
		}
	}

	int TimerSubsystem::GetFPS()
	{
		return s_TimerSubsystem->m_FramesPerSecond;
	}

	float TimerSubsystem::GetFrameTime() { return s_TimerSubsystem->m_FrameTime; }
	float TimerSubsystem::GetFrameTimeMs()
	{
		return s_TimerSubsystem->m_FrameTimeS;
	}
}
