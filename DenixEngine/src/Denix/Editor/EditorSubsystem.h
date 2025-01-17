#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Subsystem.h"
#include "Denix/Editor/EditorWidget.h"
#include "imgui.h"
#include "implot.h"

namespace Denix
{
	class SDL_GLWindow;
	class Scene;
	class SceneOrganizerWidget;
	class ActorDetailsWidget;
	class AssetBrowserWidget;
	class InputDebuggerWidget;
	class EngineProfilerWidget;
	class PerformanceSettingsWidget;
	
	class EditorSubsystem : public Subsystem<EditorSubsystem>
	{
	public:
		EditorSubsystem() = default;

		~EditorSubsystem() override = default;

		EditorSubsystem(const EditorSubsystem& _other) = delete;
		EditorSubsystem(EditorSubsystem&& _other) noexcept = delete;
		EditorSubsystem& operator=(const EditorSubsystem& _other) = delete;
		EditorSubsystem& operator=(EditorSubsystem&& _other) noexcept = delete;

		void SetActiveScene(const Ref<Scene>& _scene);
		
		static ImGuiID GetDockLeftID()  { return s_Instance->DockLeftID; }
		static ImGuiID GetDockRightID()  { return s_Instance->DockRightID; }
		static ImGuiID GetDockDownID()  { return s_Instance->DockDownID; }
	private:
		void MainMenuBar();

		void Update(float _deltaTime) override;
		void Initialize() override;
		void Deinitialize() override;

		static void NewFrame();
		static void RenderUI();
		static void ViewportUpdate();

		ImGuiID DockLeftID;
		ImGuiID DockRightID;
		ImGuiID DockDownID;

		WRef<SDL_GLWindow> m_WindowRef;
		
		bool ShowDemoWindow = false;
		bool ShowPlotDemoWindow = false;

		float m_DragSpeed;
		
		WRef<Scene> m_ActiveScene;

		Ref<SceneOrganizerWidget> m_SceneOrganizerWidget;
		
		Ref<ActorDetailsWidget> m_ActorDetailsWidget;

		Ref<AssetBrowserWidget> m_AssetBrowserWidget;

		Ref<PerformanceSettingsWidget> m_PerformanceSettingsWidget;

		Ref<EngineProfilerWidget> m_EngineProfilerWidget;

		Ref<InputDebuggerWidget> m_InputDebuggerWidget;

		friend class Engine;
		friend class EditorSubsystem;
		friend class WindowSubsystem;
	};
}
