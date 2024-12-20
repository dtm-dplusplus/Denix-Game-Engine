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

    ImGui::SeparatorText("Engine Metrics");
    ImGui::DragInt("Max FPS", &TimerSubsystem::GetMaxFPS(), 1, 0, 240);
    ImGui::Text("Program time: %.2fs", elaspedTime);
    ImGui::Text("Frame time: %.2fms", TimerSubsystem::GetFrameTimeMs());
    ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());

    ImGui::SeparatorText("Profiler Settings");
    //ImGui::DragInt("Profile Average Count", &Profile::s_AverageDurationCount, 1, 3, 100);

    ImGui::SeparatorText("Profile Sessions");
    if (!activeProfileSession){if (ImGui::Button("Begin Profiling")) ProfileSubsystem::StartProfileSession();}
    else {if (ImGui::Button("End Profiling")) ProfileSubsystem::EndProfileSession();}
        

    // Display  Profile Sessions
    for (const auto& session : profileSessions)
    {
        std::unordered_map<std::string, Ref<Profile>>& inlineProfiles = session->GetInlineProfileMap();
        static float history = 10.0f;

        ImGui::SeparatorText("Profile Data");
        ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

        const Ref<Profile>& engprofile = TimerSubsystem::Get()->m_EngineProfile;

        /*if (ImPlot::BeginPlot("Engine Frame Time", "Elapsed Time (s)", "Frame Time (ms)", ImVec2(-1, 0),
                              ImPlotFlags_None,
                              ImPlotFlags_None, ImPlotAxisFlags_AutoFit))
        {
            ImPlot::SetupAxisLimits(ImAxis_X1, elaspedTime - history, elaspedTime, ImGuiCond_Always);
            ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        }*/
        
        

        if (ImGui::TreeNode(session->GetName().c_str()))
        {
            ImGui::SeparatorText("Profile Session");
            ImGui::Text("Start Time: %f", session->GetSessionTimer()->GetStartTime());
            if (!session->IsProfiling())
            {
                ImGui::Text("End Time: %f", session->GetSessionTimer()->GetEndTime());
                ImGui::Text("Duration: %f", session->GetSessionTimer()->GetDuration());
            }
            
            ImGui::SeparatorText("Profiles");
            if (ImGui::TreeNode("Inline Profiles"))
            {
                static float yAxisScale = 0.001f; // Example scaling factor
                if (ImPlot::BeginPlot("Profile Visualizer", "Elapsed Time (s)", "Frame Time (s)", ImVec2(-1, 0),
                                  ImPlotFlags_None,
                                  ImPlotFlags_None, ImPlotAxisFlags_AutoFit))
                {
                    float sessionElapsedTime = session->IsProfiling()? elaspedTime : session->GetSessionTimer()->GetEndTime();
                    ImPlot::SetupAxisLimits(ImAxis_X1, sessionElapsedTime - history, sessionElapsedTime, ImGuiCond_Always);
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
                            ImGui::Text("Minimum Duration: %fms", profile->GetMinDurationMs());
                            ImGui::Text("Maximum Duration: %fms", profile->GetMaxDurationMs());
                            ImGui::Text("Average Duration: %fms", profile->GetAverageDurationMs());
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

                    static const char*  ilabels[]   = {"Job Count","Exec Time","Sleep Time"};

                    // Our Threads never change so we can use a static initializer
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
