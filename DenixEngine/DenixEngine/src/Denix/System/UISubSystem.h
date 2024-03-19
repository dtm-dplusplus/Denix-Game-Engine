#pragma once

#include "Subsystem.h"
#include "Denix/Core.h"

namespace Denix
{
	class UISubsystem: public Subsystem
	{
	public:
		UISubsystem();
		~UISubsystem() override;

		static UISubsystem* Get() { return s_UISubSystem; }

		void Initialize() override;

		void Deinitialize() override;

	private:
		static UISubsystem* s_UISubSystem;
	};
}
