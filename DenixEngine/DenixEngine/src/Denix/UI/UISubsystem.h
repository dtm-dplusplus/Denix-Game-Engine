#pragma once

#include "Denix/Core.h"
#include "Denix/System/SubSystem.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

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

		void Update(float _deltaTime) override;

		void NewFrame();
		void RenderUI();
		void ViewportUpdate(const Ref<SDL_GLWindow> _window);

		ImGuiID DockLeftID;
		ImGuiID DockRightID;
		ImGuiID DockDownID;
	private:
		static UISubsystem* s_UISubSystem;
	};
}