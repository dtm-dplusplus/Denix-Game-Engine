#pragma once

#include "Denix/Core.h"
#include "Denix/System/SubSystem.h"

namespace Denix
{
	class Scene;

	class EditorSubsystem : public SubSystem
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
		class WindowSubSystem* s_WindowSubSystem;
		class SceneSubSystem* s_SceneSubSystem;

		Ref<Scene> m_ActiveScene;

		// TEMP ImGui
		int ObjectSelection = 0;
		bool ScenePanelOpen = true;
		bool ShowDemoWindow = false;

		float DragSpeed = 1.0f;
		float DragSpeedDelta;

		void ScenePanel();
		void DetailsPanel();
		void MenuBar();
	};

}
