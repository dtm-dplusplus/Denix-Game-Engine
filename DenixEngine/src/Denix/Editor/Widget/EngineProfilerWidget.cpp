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
    DE_PROFILE(EngineProfilerWidget)
    EditorWidget::Update(_deltaTime);

    ImGui::Begin("Profiler");

    //ImGui::DragInt("Max FPS", &TimerSubsystem::GetMaxFPS(), 1, 0, 240);
    ImGui::SeparatorText("Engine Metrics");
    ImGui::Text("Program time: %fms", TimerSubsystem::GetElapsedTime());
    ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTime());
    ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());

    ImGui::DragInt("Profile Average Count", &Profile::s_AverageDurationCount, 1, 3, 100);

    ImGui::SeparatorText("Profile Sessions");
    // if (ImGui::Button("Clear Profile Data")) ProfileSubsystem::Get()->m_ClearProfiles = true;
    Ref<ProfileSession> activeProfileSession = ProfileSubsystem::GetActiveProfileSession();
   std::vector<Ref<ProfileSession>>& profileSessions = ProfileSubsystem::Get()->m_ProfileSessions;

    // Begin or End Profiling
    if (!activeProfileSession)
    {
        if (ImGui::Button("Begin Profiling")) ProfileSubsystem::StartProfileSession();
    }
    else
    {
        if (ImGui::Button("End Profiling")) ProfileSubsystem::EndProfileSession();
    }
    
    // Display Active Profile Session
    if (activeProfileSession)
    {
        const float elaspedTime = TimerSubsystem::GetElapsedTime();
        std::unordered_map<std::string, Profile>& profiles = activeProfileSession->GetProfiles();
        static float history = 10.0f;

        // Use these to adjust the plot axis
        // static float profileMinDuration = 0.0f;
        // static float profileMaxDuration = 1.0f;
        
        ImGui::SeparatorText("Profile Data");
        ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

       const Ref<Profile>& engprofile = TimerSubsystem::Get()->m_EngineProfile;

        if (ImPlot::BeginPlot("Profile Visualizer", "Elapsed Time (s)", "Frame Time (ms)", ImVec2(-1, 0), ImPlotFlags_None,
                              ImPlotFlags_None, ImPlotAxisFlags_AutoFit))
        {
            ImPlot::SetupAxisLimits(ImAxis_X1, elaspedTime - history, elaspedTime, ImGuiCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, engprofile->m_AverageDuration * 1.25f, ImGuiCond_Always);
            ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
            for (const auto& [name, profile] : profiles)
            {
                if (profile.m_Buffer.ProfileResults.empty()) continue;
                    ImPlot::PlotLine(name.c_str(), &profile.m_Buffer.ProfileResults[0].x, &profile.m_Buffer.ProfileResults[0].y,
                                     profile.m_Buffer.ProfileResults.size(), 0, 0, 2 * sizeof(float));

                // Update the plot axis
                //profileMinDuration = std::min(profileMinDuration, profile.m_MinimumDuration);
                //profileMaxDuration = std::max(profileMaxDuration, profile.m_MaximumDuration);
            }
            ImPlot::EndPlot();
        }

        if (ImGui::CollapsingHeader("Active Profile Session"))
        {
            for (const auto& [name, profile] : profiles)
            {
                if (ImGui::TreeNode(name.c_str()))
                {
                    ImGui::Text("Minimum Duration: %fms", profile.m_MinimumDuration);
                    ImGui::Text("Maximum Duration: %fms", profile.m_MaximumDuration);
                    ImGui::Text("Average Duration: %fms", profile.m_AverageDuration);
                    ImGui::TreePop();
                }
            }
        }
    }

    // Display Past Profile Sessions
    for (const auto& session : profileSessions)
    {
        if (ImGui::TreeNode(session->GetName().c_str()))
        {
            for (const auto& [name, profile] : session->GetProfiles())
            {
                if (ImGui::TreeNode(name.c_str()))
                {
                    ImGui::Text("Minimum Duration: %fms", profile.m_MinimumDuration);
                    ImGui::Text("Maximum Duration: %fms", profile.m_MaximumDuration);
                    ImGui::Text("Average Duration: %fms", profile.m_AverageDuration);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }

    ImGui::End();
    DE_PROFILE_END(EngineProfilerWidget)
}
