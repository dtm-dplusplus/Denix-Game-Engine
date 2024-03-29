#pragma once

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"

namespace Denix
{
	class Scene;

	class EditorSubsystem : public Subsystem
	{
	public:
		EditorSubsystem()
		{
			s_EditorSubsystem = this;
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

	private:
		static EditorSubsystem* s_EditorSubsystem;
		class WindowSubsystem* s_WindowSubSystem;
		class SceneSubsystem* s_SceneSubSystem;
		class InputSubsystem* s_InputSubsystem;
		class RendererSubsystem* s_RendererSubSystem;

		Ref<Scene> m_ActiveScene;

		// TEMP ImGui
		int ObjectSelection = 0;
		bool ScenePanelOpen = true;
		bool ShowDemoWindow = false;
		float WinX = 0.0f;
		float WinY = 0.0f;

		float DragSpeed = 1.0f;
		float DragSpeedDelta;

		// UI
		void ScenePanel();
		bool m_IsScenePanelOpen = true;

		void DetailsPanel();
		bool m_IsDetailsPanelOpen = true;

		// Input
		bool m_IsInputPanelOpen = false;

		// Menu Bar
		void MenuBar();
		float MenuBarHeight = 15.f;
		float ViewportBarHeight = 15.f;
	};

}
