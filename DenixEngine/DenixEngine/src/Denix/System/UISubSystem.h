#pragma once

#include "SubSystem.h"
#include "Denix/Core.h"

namespace Denix
{
	class UISubSystem: public SubSystem
	{
	public:
		UISubSystem();
		~UISubSystem() override;

		static UISubSystem* Get() { return s_UISubSystem; }

		void Initialize() override;

		void Deinitialize() override;

	private:
		static UISubSystem* s_UISubSystem;
	};
}