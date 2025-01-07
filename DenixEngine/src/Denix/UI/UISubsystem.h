#pragma once

#include "Denix/Core.h"
#include "Denix/System/SubSystem.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "implot.h"
#include "implot_internal.h"

namespace Denix
{
	class SDL_GLWindow;

	class UISubsystem: public Subsystem<UISubsystem>
	{
	public:
		UISubsystem();
		~UISubsystem() override = default;

		UISubsystem(const UISubsystem& _other) = delete;
		UISubsystem(UISubsystem&& _other) noexcept = delete;
		UISubsystem& operator=(const UISubsystem& _other) = delete;
		UISubsystem& operator=(UISubsystem&& _other) noexcept = delete;

		static ImGuiID GetDockLeftID()  { return s_Instance->DockLeftID; }
		static ImGuiID GetDockRightID()  { return s_Instance->DockRightID; }
		static ImGuiID GetDockDownID()  { return s_Instance->DockDownID; }
	private:
		void Initialize() override;

		void Deinitialize() override;

		void Update(float _deltaTime) override;

		static void NewFrame();
		static void RenderUI();
		void ViewportUpdate() const;

		ImGuiID DockLeftID;
		ImGuiID DockRightID;
		ImGuiID DockDownID;

		WRef<SDL_GLWindow> m_WindowRef;
		friend class Engine;
		friend class EditorSubsystem;
	};
}