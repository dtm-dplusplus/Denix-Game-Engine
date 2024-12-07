#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "EngineProfilerWidget.h"

#include "Denix/Core/TimerSubsystem.h"
#include "Denix/UI/UISubsystem.h"

#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"

Denix::EngineProfilerWidget::EngineProfilerWidget(): EditorWidget({"Engine Profiler"})
{
}

void Denix::EngineProfilerWidget::Update(float _deltaTime)
{
    EditorWidget::Update(_deltaTime);

    ImGui::Begin("Profiler");

    const auto& profiles = ProfileSubsystem::Get()->GetProfiles();
    const float elaspedTime = TimerSubsystem::GetProgramElapsedTime();

    //ImGui::DragInt("Max FPS", &TimerSubsystem::GetMaxFPS(), 1, 0, 240);
    ImGui::Text("Program time: %fms", TimerSubsystem::GetProgramElapsedTime());
    ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTime());
    ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());

    if (ImGui::Button("Clear Profile Data")) ProfileSubsystem::Get()->m_ClearProfiles = true;
    
    static float history = 5.0f;
    ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

    const Ref<Profile>& engprofile = TimerSubsystem::Get()->m_EngineProfile;

    if (ImPlot::BeginPlot("Profile Visualizer", nullptr, "Frame Time (ms)", ImVec2(-1, 0), ImPlotFlags_None,
                          ImPlotFlags_None, ImPlotAxisFlags_AutoFit))
    {
        ImPlot::SetupAxisLimits(ImAxis_X1, elaspedTime - history, elaspedTime, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, engprofile->m_AverageDuration * 1.5f, ImGuiCond_Always);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        for (const auto& [name, profile] : profiles)
        {
            if (profile->m_Buffer.Data.size() > 0)
                ImPlot::PlotLine(name.c_str(), &profile->m_Buffer.Data[0].x, &profile->m_Buffer.Data[0].y,
                                 profile->m_Buffer.Data.size(), 0, profile->m_Buffer.Offset, 2 * sizeof(float));
        }
        ImPlot::EndPlot();
    }

    for (const auto& [name, profile] : profiles)
    {
        if (ImGui::TreeNode(name.c_str()))
        {
            //ImGui::Text("Frame Percentage: %.2f%%", profile->m_FramePercentage * 100.0f);
            ImGui::Text("Duration: %fms", profile->GetDuration());
            ImGui::Text("Minimum Duration: %fms", profile->m_MinimumDuration);
            ImGui::Text("Maximum Duration: %fms", profile->m_MaximumDuration);
            ImGui::Text("Average Duration: %fms", profile->m_AverageDuration);
            if (ImGui::DragInt("Average Duration Count", &profile->m_AverageDurationCount, 1.0f, 3, 100))
            {
                profile->m_DurationRecords.resize(profile->m_AverageDurationCount);
            }
            ImGui::TreePop();
        }
    }

    ImGui::End();
}
