#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Subsystem.h"
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

	private:
		void MainMenuBar();

		void Update(float _deltaTime) override;
		void Initialize() override;
		void Deinitialize() override;
		
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
	};
}
