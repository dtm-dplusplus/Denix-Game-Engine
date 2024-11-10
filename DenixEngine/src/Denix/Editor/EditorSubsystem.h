#pragma once

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"


namespace Denix
{
	struct ShaderSource;
	class ShaderEditor;
	class Camera;
	class GameObject;
	class Scene;
	class Material;
	class SceneOrganizerWidget;
	class GameObjectDetailsWidget;
	class AssetBrowserWidget;

	class EditorSubsystem : public Subsystem
	{
	public:
		EditorSubsystem()
		{
			s_EditorSubsystem = this;
			DE_LOG_CREATE(LogEditor)
		}

		~EditorSubsystem() override
		{
			s_EditorSubsystem = nullptr;
		}

		void SetActiveScene(const Ref<Scene>& _scene);

	public:
		static EditorSubsystem* Get() { return s_EditorSubsystem; }
		void Update(float _deltaTime) override;
		void Initialize() override;
		void Deinitialize() override;

		// Scene Viewer
		void ScenePropertiesWidget() const;

	private:
		static EditorSubsystem* s_EditorSubsystem;
		class WindowSubsystem* s_WindowSubsystem;
		class SceneSubsystem* s_SceneSubsystem;
		class InputSubsystem* s_InputSubsystem;
		class UISubsystem* s_UISubsystem;

		Ref<Scene> m_ActiveScene;

		Ref<SceneOrganizerWidget> m_SceneOrganizerWidget;
		Ref<GameObjectDetailsWidget> m_GameObjectDetailsWidget;

		Ref<AssetBrowserWidget> m_AssetBrowserWidget;
		
		bool ShowDemoWindow = false;
		bool ShowPlotDemoWindow = false;
		float WinX = 0.0f;
		float WinY = 0.0f;

		float DragSpeed = 10.0f;
		float DragSpeedDelta;

		// Profiler
		bool m_IsProfilerOpen = false;
		void Profiler();
		
		// UI
		void SceneWidgets();

		// Input
		bool m_IsInputPanelOpen = false;

		// Physics
		bool m_IsPhysicsSettingsOpen = false;
		void PhysicsSettings();

		// Timer
		bool m_IsTimerSettingsOpen = false;
		static void TimerSettings();

		// Menu Bar
		void MenuBar();
		float MenuBarHeight = 15.f;
		float ViewportBarHeight = 15.f;
	};
}
