
#include "EngineProfilerWidget.h"

#include "JobTableWidget.h"
#include "PerformanceSettingsWidget.h"
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
    const std::vector<Ref<ProfileSession>>& profileSessions = ProfileSubsystem::GetProfileSessions();

    ImGui::SetNextWindowSize({600, 800}, ImGuiCond_FirstUseEver);
    ImGui::Begin("Profiler Widget");
    ImGui::SeparatorText("Profiler");
    if (!ProfileSubsystem::GetActiveProfileSession())
    {
        if (ImGui::Button("Begin Profiling")) ProfileSubsystem::StartProfileSession();
    }
    else
    {
        if (ImGui::Button("End Profiling")) ProfileSubsystem::EndProfileSession();
    }
        
    ImGui::SeparatorText("Engine Metrics");
    PerformanceSettingsWidget::Show();

    ImGui::SeparatorText("Profile Sessions");

    // Display  Profile Sessions
    for (const auto& session : profileSessions)
    {
        std::unordered_map<std::string, Ref<Profile>>& inlineProfiles = session->GetInlineProfileMap();

        if (ImGui::TreeNode(session->GetName().c_str()))
        {
            ImGui::SliderFloat("History", &session->GetGraphHistory(), 1, session->GetDuration(), "%.1f s");

            ImGui::SeparatorText("Profile Session");
            ImGui::Text("Start Time: %fs", session->GetSessionTimer()->GetStartTime());
            if (!session->IsProfiling())
            {
                ImGui::Text("End Time: %fs", session->GetSessionTimer()->GetEndTime());
                ImGui::Text("Duration: %fs", session->GetSessionTimer()->GetDuration());
                ImGui::Text("Average FPS: %d", session->GetAverageFramesPerSecond());
                ImGui::Text("Average Frame Time: %.2fms", session->GetAverageFrameTimeMs());
                ImGui::Text("Min Frame Time: %.2fms", session->GetMinFrameTimeMs());
                ImGui::Text("Max Frame Time: %.2fms", session->GetMaxFrameTimeMs());
            }
            
            ImGui::SeparatorText("Profiles");
            if (ImGui::TreeNode("Inline Profiles"))
            {
                if (ImPlot::BeginPlot("Profile Visualizer", "Elapsed Time (s)", "Frame Time (s)", ImVec2(-1, 300),
                    ImPlotFlags_None, ImPlotAxisFlags_None, ImPlotAxisFlags_None))
                {
                    const float sessionElapsedTime = session->IsProfiling()? elaspedTime : session->GetSessionTimer()->GetEndTime();
                    const float yMax = session->IsProfiling()? TimerSubsystem::GetAverageFrameTime() : session->GetAverageFrameTime();
                    ImPlot::SetupAxisLimits(ImAxis_X1, sessionElapsedTime - session->GetGraphHistory(), sessionElapsedTime, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, yMax, ImGuiCond_Always);
                    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);

                    // Apply a scaling factor to the Y-axis
                    for (const auto& [name, profile] : inlineProfiles)
                    {
                        if (profile->m_DurationBuffer.ProfileResults.empty()) continue;
                        ImPlot::PlotLine(name.c_str(), &profile->m_DurationBuffer.ProfileResults[0].EndTime,
                                         &profile->m_DurationBuffer.ProfileResults[0].Duration,
                                         profile->m_DurationBuffer.ProfileResults.size(), 0, 0, 3 * sizeof(float));

                        if (ImGui::TreeNode(name.c_str()))
                        {
                            ImGui::Text("Minimum Duration: %.2fms", profile->GetMinDurationMs());
                            ImGui::Text("Maximum Duration: %.2fms", profile->GetMaxDurationMs());
                            ImGui::Text("Average Duration: %.2fms", profile->GetAverageDurationMs());
                            ImGui::TreePop();
                        }
                    }
                    ImPlot::EndPlot();
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
                if (!session->IsProfiling())
                {
                    std::vector<ThreadData>& threadData = session->GetThreadData();

                    static const char*  ilabels[]   = {"Job Count","Exec Time","Sleep Time"};

                    // Our System Threads never change so we can use a static initializer
                    static std::vector<std::string>  xLabelsStr   = [threadData] {
                        std::vector<std::string> labels;
                        for (const auto& thread : threadData)
                            labels.push_back("TID" + std::to_string(thread.m_ThreadID));
                    
                        return labels;
                    }();
                    
                    // C Array for imgui
                    static std::vector<const char*> xLabels = [] {
                        std::vector<const char*> labels;
                        for (const auto& label : xLabelsStr)
                            labels.push_back(label.c_str());
                    
                        return labels;
                    }();

                    static std::vector<double> positions = [] {
                        std::vector<double> pos;
                        for (int i = 0; i < xLabels.size(); i++)
                            pos.push_back(i);

                        return pos;
                    }();

                    static int graphType =0;
                    static int groups = xLabels.size();
                    static float size = 0.67f;
                    ImGui::Text("graph type %d",graphType);
                    
                    if (ImGui::BeginCombo("Graph Type", ilabels[graphType]))
                    {
                        for (int i = 0; i < IM_ARRAYSIZE(ilabels); i++)
                        {
                            bool is_selected = (graphType == i);
                            if (ImGui::Selectable(ilabels[i], is_selected))
                                graphType = i;
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::SameLine();

                    ImGui::SliderFloat("Bar Group Size",&size,0,1);
                    
                        static ImPlotBarGroupsFlags flags = 0;
                        
                        switch (graphType)
                        {
                            case 0:
                            {
                                if (ImPlot::BeginPlot("Thread Job Count")) {
                                    ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
                                    ImPlot::SetupAxes("Threads","Count",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
                                    ImPlot::SetupAxisTicks(ImAxis_X1,&positions[0], groups, &xLabels[0]);
                                    ImPlot::PlotBars(ilabels[graphType],&session->GetThreadJobCounts()[0],groups,size,0,flags);
                                    ImPlot::EndPlot();
                                }
                                break;
                            }

                        case 1:
                                {
                                    if (ImPlot::BeginPlot("Thread Job Execution Time")) {
                                        ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
                                        ImPlot::SetupAxes("Threads","Time (s)",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
                                        ImPlot::SetupAxisTicks(ImAxis_X1,&positions[0], groups, &xLabels[0]);
                                        ImPlot::PlotBars(ilabels[graphType],&session->GetThreadJobExecTimes()[0],groups,size,0,flags);
                                        ImPlot::EndPlot();
                                    }
                                }
                            break;

                        case 2:
                            {
                                if (ImPlot::BeginPlot("Thread Sleep Time")) {
                                    ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
                                    ImPlot::SetupAxes("Threads","Time (s)",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
                                    ImPlot::SetupAxisTicks(ImAxis_X1,&positions[0], groups, &xLabels[0]);
                                    ImPlot::PlotBars(ilabels[graphType],&session->GetThreadSleepTimes()[0],groups,size,0,flags);
                                    ImPlot::EndPlot();
                                }
                            }
                        default: break;
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
