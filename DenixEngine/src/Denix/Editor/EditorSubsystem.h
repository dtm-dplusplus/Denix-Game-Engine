#pragma once

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"
#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
	class Scene;
	class SceneOrganizerWidget;
	class ActorDetailsWidget;
	class AssetBrowserWidget;
	class InputDebuggerWidget;
	class EngineProfilerWidget;
	class PerformanceSettingsWidget;
	
	class EditorSubsystem : public Subsystem
	{
	public:
		EditorSubsystem();

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

		void MainMenuBar();

		bool ShowDemoWindow = false;
		bool ShowPlotDemoWindow = false;

		float m_DragSpeed;
		
		Ref<Scene> m_ActiveScene;


		Ref<SceneOrganizerWidget> m_SceneOrganizerWidget;
		
		Ref<ActorDetailsWidget> m_ActorDetailsWidget;

		Ref<AssetBrowserWidget> m_AssetBrowserWidget;

		Ref<PerformanceSettingsWidget> m_PerformanceSettingsWidget;

		Ref<EngineProfilerWidget> m_EngineProfilerWidget;

		Ref<InputDebuggerWidget> m_InputDebuggerWidget;
	};
}
