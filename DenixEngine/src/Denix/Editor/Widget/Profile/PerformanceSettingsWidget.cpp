
#include "PerformanceSettingsWidget.h"
#include "Denix/UI/UISubsystem.h"
#include "Denix/Core/Time/TimerSubsystem.h"

Denix::PerformanceSettingsWidget::PerformanceSettingsWidget(): EditorWidget({"Performance Settings"})
{
}

void Denix::PerformanceSettingsWidget::Update(float _deltaTime)
{
    EditorWidget::Update(_deltaTime);

    ImGui::Begin(GetName().c_str());
    ImGui::SeparatorText("Engine Metrics");
    Show();
    ImGui::End();
}

void Denix::PerformanceSettingsWidget::Show()
{
    ImGui::DragInt("Max FPS", &TimerSubsystem::GetMaxFPS(), 1, 0, 240);
    ImGui::SliderFloat("Game Speed", &TimerSubsystem::GetGameTimeSpeed(), 0.0f, 2.0f);
    ImGui::Text("Program time: %.2fs", TimerSubsystem::GetProgramElaspedTime());
    ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());
    ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTimeMs());
}
