#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "PerformanceSettingsWidget.h"
#include "Denix/UI/UISubsystem.h"
#include "Denix/Core/TimerSubsystem.h"

Denix::PerformanceSettingsWidget::PerformanceSettingsWidget(): EditorWidget({"Performance Settings"})
{
}

void Denix::PerformanceSettingsWidget::Update(float _deltaTime)
{
    EditorWidget::Update(_deltaTime);

    ImGui::Begin(GetName().c_str());
    ImGui::DragInt("Max FPS", &TimerSubsystem::GetMaxFPS(), 1, 0, 240);
    ImGui::SliderFloat("Game Speed", &TimerSubsystem::GetGameTimeSpeed(), 0.0f, 2.0f);
    ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTimeMs());
    ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());
    ImGui::End();
}
