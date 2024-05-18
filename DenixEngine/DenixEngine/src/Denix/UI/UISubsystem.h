#pragma once

#include "Denix/Core.h"
#include "Denix/System/SubSystem.h"

namespace Denix
{
	class SDL_GLWindow;

	class UISubsystem: public Subsystem
	{
	public:
		UISubsystem();
		~UISubsystem() override;

		static UISubsystem* Get() { return s_UISubSystem; }

		void Initialize() override;

		void Deinitialize() override;

		void NewFrame();
		void RenderUI();
		void ViewportUpdate(const Ref<SDL_GLWindow> _window);
	private:
		static UISubsystem* s_UISubSystem;
	};
}