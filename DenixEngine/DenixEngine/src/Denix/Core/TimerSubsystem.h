#pragma once

#include "Denix/System/Subsystem.h"
#include "Denix/Core/Logger.h"

namespace Denix
{
	class TimerSubsystem : public Subsystem
	{
	public:
		TimerSubsystem()
		{
			s_TimerSubsystem = this;

			DE_LOG_CREATE(LogTimer)
		}

		~TimerSubsystem() override
		{
			s_TimerSubsystem = nullptr;
		}

		static TimerSubsystem* Get() { return s_TimerSubsystem; }

		void Initialize() override
		{
			DE_LOG(LogTimer, Trace, "TimerSubsystem Initialized")

			m_Initialized = true;
		}

		void Deinitialize() override
		{
			DE_LOG(LogTimer, Trace, "TimerSubsystem Deinitialized")
			m_Initialized = false;
		}

	private:
		static TimerSubsystem* s_TimerSubsystem;

	};
}
