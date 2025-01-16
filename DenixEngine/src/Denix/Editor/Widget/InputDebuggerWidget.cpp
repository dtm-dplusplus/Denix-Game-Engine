
#include "InputDebuggerWidget.h"

#include "Denix/UI/UISubsystem.h"
#include "Denix/Input/InputSubsystem.h"

Denix::InputDebuggerWidget::InputDebuggerWidget(): EditorWidget({"Input Debugger"})
{
    
}

void Denix::InputDebuggerWidget::Update(float _deltaTime)
{
    MouseData mouseData = InputSubsystem::GetMouseData();
    
    ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_Appearing);
    
    ImGui::Begin(GetName().c_str());

    ImGui::BeginDisabled();
    ImGui::SeparatorText("Mouse Motion");
    ImGui::Text("Mouse Pos: %f %f", mouseData.X, mouseData.Y);
    ImGui::Text("Mouse Rel: %f %f", mouseData.RelX, mouseData.RelY);
    ImGui::Text("Mouse Wheel: %f", mouseData.WheelY);
    ImGui::SeparatorText("Mouse Buttons");
    ImGui::Checkbox("Left", &mouseData.Left);
    ImGui::Checkbox("Right", &mouseData.Right);
    ImGui::Checkbox("Middle", &mouseData.Middle);
    ImGui::Checkbox("Side1", &mouseData.Side1);
    ImGui::Checkbox("Side2", &mouseData.Side2);
    ImGui::EndDisabled();

    ImGui::End();
}
