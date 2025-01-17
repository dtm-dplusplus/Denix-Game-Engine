
#include "SceneSettingsWidget.h"

#include "imgui.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/UI/UISubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Video/RendererSubsystem.h"

Denix::SceneSettingsWidget::SceneSettingsWidget(const WRef<Scene>& _scene): SceneEditorWidget({"Scene Settings"}, _scene)
{
}

void Denix::SceneSettingsWidget::Update(float _deltaTime)
{
    SceneEditorWidget::Update(_deltaTime);

    ImGui::SetNextWindowDockID(EditorSubsystem::GetDockLeftID(), ImGuiCond_Appearing);
    ImGui::Begin(GetName().c_str());

    ImGui::SeparatorText("Scene");

    // Viewport Mode
    //ImGui::Combo("Viewport Mode", &RendererSubsystem::GetViewportMode(), "Default\0Unlit\0Wireframe\0Collision\0\0");
		
    ImGui::SeparatorText("Physics");
    ImGui::DragFloat("Gravity", &m_SceneRef.lock()->GetGravity());
    ImGui::Checkbox("Collision Detection", &PhysicsSubsystem::CollisionDetectionEnabledRef());
    ImGui::Checkbox("Collision Response", &PhysicsSubsystem::CollisionResponseEnabledRef());

    ImGui::End();
}
