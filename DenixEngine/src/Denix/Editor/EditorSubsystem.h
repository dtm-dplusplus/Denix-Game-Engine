#pragma once

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"
#include "Denix/Video/GL/Shader.h"

namespace Denix
{
	struct ShaderSource;
	class Camera;
	class GameObject;
	class Scene;
	class Material;

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

		struct ShaderEditorWidget
		{
			Ref<Shader> Shader;
			bool IsOpen;
			void WidgetEditor()
			{
				ImGui::SetNextWindowSize(ImVec2(1000, 500), ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowFocus();
				ImGui::Begin("Shader Editor", &IsOpen);
				ImGui::BeginTabBar("Shader Editor Tabs");
				if (Shader)
				{
					for (auto& shaderSources = Shader->GetShaderSources(); auto& shaderSource : shaderSources)
					{
						if (ImGui::BeginTabItem(shaderSource.FileName.c_str()))
						{
							ImGui::BeginChild(shaderSource.FileName.c_str(), ImVec2(0, 0), true);
							ImGui::Text(shaderSource.Source.c_str());
							ImGui::EndChild();
							ImGui::EndTabItem();
						}
					}
				}
				else
				{
					ImGui::Text("No shader selected");
				}
				ImGui::EndTabBar();
				ImGui::End();
			}
		};

	public:
		static EditorSubsystem* Get() { return s_EditorSubsystem; }
		void Update(float _deltaTime) override;
		void Initialize() override;
		void Deinitialize() override;

	private:
		static EditorSubsystem* s_EditorSubsystem;
		class WindowSubsystem* s_WindowSubsystem;
		class SceneSubsystem* s_SceneSubsystem;
		class InputSubsystem* s_InputSubsystem;
		class RendererSubsystem* s_RendererSubsystem;
		class UISubsystem* s_UISubsystem;

		Ref<Scene> m_ActiveScene;

		Ref<ShaderEditorWidget> m_ShaderEditorWidget;
		// TEMP ImGui
		int m_ObjectSelection = 0;
		bool ScenePanelOpen = true;
		bool ShowDemoWindow = false;
		float WinX = 0.0f;
		float WinY = 0.0f;

		float DragSpeed = 10.0f;
		float DragSpeedDelta;

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
		void TimerSettings();

		// Menu Bar
		void MenuBar();
		float MenuBarHeight = 15.f;
		float ViewportBarHeight = 15.f;

		// Scene Viewer
		void ScenePropertiesWidget() const;
		void SceneAddObjectWidget();
		void SceneOrganizerWidget();
		void LightWidget(const Ref<GameObject>& _selectedObject) const;
		void PhysicsWidget(const Ref<GameObject>& _selectedObject) const;
		void CollisionWidget(const Ref<GameObject>& _selectedObject) const;

		void RenderWidget(const Ref<GameObject>& _selectedObject);
		void MaterialWidget(const Ref<GameObject>& _selectedObject);
		void MaterialSelectionWidget(Ref<Material>& _material);
		void TextureSelectionWidget(Ref<Material>& _material);
		void ShaderSelectionWidget(Ref<Material>& _material);
		void MeshWidget(const Ref<GameObject>& _selectedObject);
		void TransformWidget(const Ref<GameObject>& _object) const;
		void CameraWidget(const Ref<GameObject>& _camera) const;
	};
}
