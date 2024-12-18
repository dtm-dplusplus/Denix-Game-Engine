#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "EngineProfilerWidget.h"

#include "JobTableWidget.h"
#include "Denix/Core/TimerSubsystem.h"
#include "Denix/UI/UISubsystem.h"

#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"


Denix::EngineProfilerWidget::EngineProfilerWidget(): EditorWidget({"Engine Profiler"})
{
    m_JobTableWidget = MakeRef<JobTableWidget>();
}

void Denix::EngineProfilerWidget::Update(float _deltaTime)
{
    EditorWidget::Update(_deltaTime);

    const float elaspedTime = Timer::GetProgramElaspedTime();
    Ref<ProfileSession> activeProfileSession = ProfileSubsystem::GetActiveProfileSession();
    std::vector<Ref<ProfileSession>>& profileSessions = ProfileSubsystem::Get()->m_ProfileSessions;

    ImGui::Begin("Profiler Widget");

    ImGui::SeparatorText("Performance Settings");
    ImGui::DragInt("Max FPS", &TimerSubsystem::GetMaxFPS(), 1, 0, 240);
    ImGui::SeparatorText("Engine Metrics");
    ImGui::Text("Program time: %fs", elaspedTime);
    ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTimeMs());
    ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());

    ImGui::SeparatorText("Profiler Settings");
    //ImGui::DragInt("Profile Average Count", &Profile::s_AverageDurationCount, 1, 3, 100);

    ImGui::SeparatorText("Profile Sessions");
    if (!activeProfileSession){if (ImGui::Button("Begin Profiling")) ProfileSubsystem::StartProfileSession();}
    else {if (ImGui::Button("End Profiling")) ProfileSubsystem::EndProfileSession();}
        

    // Display  Profile Sessions
    for (const auto& session : profileSessions)
    {
        std::unordered_map<std::string, Profile>& inlineProfiles = session->GetInlineProfiles();
        static float history = 10.0f;

        ImGui::SeparatorText("Profile Data");
        ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

        const Ref<Profile>& engprofile = TimerSubsystem::Get()->m_EngineProfile;

        if (session->IsProfiling())
        {
            static float yAxisScale = 1000.0f; // Example scaling factor
            if (ImPlot::BeginPlot("Profile Visualizer", "Elapsed Time (s)", "Frame Time (ms)", ImVec2(-1, 0),
                              ImPlotFlags_None,
                              ImPlotFlags_None, ImPlotAxisFlags_AutoFit))
            {
                ImPlot::SetupAxisLimits(ImAxis_X1, elaspedTime - history, elaspedTime, ImGuiCond_Always);
                // ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, engprofile->GetAverageDurationMs() * 1.25f, ImGuiCond_Always);
                ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);

                // Apply a scaling factor to the Y-axis
           
                ImPlot::SetupAxisScale(ImAxis_Y1, yAxisScale);
            
                for (const auto& [name, profile] : inlineProfiles)
                {
                    if (profile.m_DurationBuffer.ProfileResults.empty()) continue;
                    ImPlot::PlotLine(name.c_str(), &profile.m_DurationBuffer.ProfileResults[0].StartTime,
                                     &profile.m_DurationBuffer.ProfileResults[0].Duration,
                                     profile.m_DurationBuffer.ProfileResults.size(), 0, 0, 3 * sizeof(float));
                }
                ImPlot::EndPlot();
            }
        }

        if (ImGui::TreeNode(session->GetName().c_str()))
        {
            if (ImGui::TreeNode("Inline Profiles"))
            {
                for (const auto& [name, profile] : inlineProfiles)
                {
                    if (ImGui::TreeNode(name.c_str()))
                    {
                        ImGui::Text("Minimum Duration: %fms", profile.GetMinDurationMs());
                        ImGui::Text("Maximum Duration: %fms", profile.GetMaxDurationMs());
                        ImGui::Text("Average Duration: %fms", profile.GetAverageDurationMs());
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Job Profiles"))
            {
                m_JobTableWidget->m_ProfileSession = session;
                m_JobTableWidget->Update(_deltaTime);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Thread Profiles"))
            {
                // Calc Load Difference
                /*float jobLoadMin = threads[0]->m_JobExecCount, jobLoadMax = 0;
                float execTimeMin = threads[0]->m_ThreadExecTime, execTimeMax = 0;

                for (const auto& thread : threads)
                {
                    if (thread->m_JobExecCount > jobLoadMax) jobLoadMax = thread->m_JobExecCount;
                    if (thread->m_JobExecCount < jobLoadMin) jobLoadMin = thread->m_JobExecCount;

                    if (thread->m_ThreadExecTime > execTimeMax) execTimeMax = thread->m_ThreadExecTime;
                    if (thread->m_ThreadExecTime < execTimeMin) execTimeMin = thread->m_ThreadExecTime;
                }*/

                /*float jobLoadDiff = 1.0f - jobLoadMin / jobLoadMax;
                float execTimeDiff = execTimeMin / execTimeMax;
                ImGui::Text("Load Diff: %f%", jobLoadDiff * 100.0f);
                ImGui::Text("Exec Diff: %f%", execTimeDiff * 100.0f);*/

                if (!session->IsProfiling())
                {
                    std::vector<ThreadData>& threadData = session->GetThreadData();
                    for (const auto& thread : threadData)
                    {
                        ImGui::Text("ID: %d, Jobs Executed: %d, Thread Exec Time: %f, Thread Sleep Time: %f",
                                    thread.m_ThreadID, thread.m_JobExecCount, thread.m_ThreadExecTime,
                                    thread.m_ThreadSleepTime);
                    }
                }
                else
                {
                    ImGui::NewLine();
                    ImGui::Text("Finish Profiling to view results");                        
                }
                
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }
    ImGui::End();
}
