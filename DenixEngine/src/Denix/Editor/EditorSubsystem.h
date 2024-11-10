#pragma once

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"
#include "Denix/Video/GL/Shader.h"
#include "Denix/Video/Renderer/RenderComponent.h"


namespace Denix
{
	struct ShaderSource;
	class ShaderEditor;
	class Camera;
	class GameObject;
	class Scene;
	class Material;
	class SceneOrganizerWidget;
	
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
		void SceneAddObjectWidget();
		//void SceneOrganizerWidget();
		void LightWidget(const Ref<GameObject>& _selectedObject) const;
		void PhysicsWidget(const Ref<GameObject>& _selectedObject) const;
		void CollisionWidget(const Ref<GameObject>& _selectedObject) const;

		void RenderWidget(const Ref<GameObject>& _selectedObject);
		void MaterialWidget(const Ref<GameObject>& _selectedObject);
		void MaterialSelectionWidget(Ref<RenderComponent>& _rendComp);
		void TextureSelectionWidget(const Ref<Material>& _material);
		void ShaderSelectionWidget(Ref<Material>& _material);
		void MeshWidget(const Ref<GameObject>& _selectedObject);
		void TransformWidget(const Ref<GameObject>& _object) const;
		void CameraWidget(const Ref<GameObject>& _camera) const;
	private:
		static EditorSubsystem* s_EditorSubsystem;
		class WindowSubsystem* s_WindowSubsystem;
		class SceneSubsystem* s_SceneSubsystem;
		class InputSubsystem* s_InputSubsystem;
		class RendererSubsystem* s_RendererSubsystem;
		class UISubsystem* s_UISubsystem;

		Ref<Scene> m_ActiveScene;

		Ref<ShaderEditor> m_ShaderEditor;
		Ref<SceneOrganizerWidget> m_SceneOrganizerWidget;
		
		// TEMP ImGui
		int m_ObjectSelection = 0;
		bool ScenePanelOpen = true;
		bool ShowDemoWindow = false;
		bool ShowPlotDemoWindow = false;
		float WinX = 0.0f;
		float WinY = 0.0f;

		float DragSpeed = 10.0f;
		float DragSpeedDelta;
		bool m_IsAssetBrowserOpen = false;

		// Assets
		void AssetBrowser();

		// Profiler
		bool m_IsProfilerOpen = true;
		void Profiler();
		
		// UI
		void ScenePanel();
		bool m_IsScenePanelOpen = true;

		void DetailsPanel();
		bool m_IsDetailsPanelOpen = true;

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
