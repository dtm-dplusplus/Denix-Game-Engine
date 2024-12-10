#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "EngineProfilerWidget.h"

#include "Denix/Core/TimerSubsystem.h"
#include "Denix/UI/UISubsystem.h"

#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"



ImGuiTableSortSpecs* MyItem::s_current_sort_specs = NULL;



Denix::EngineProfilerWidget::EngineProfilerWidget(): EditorWidget({"Engine Profiler"})
{
}

void Denix::EngineProfilerWidget::Update(float _deltaTime)
{
    DE_PROFILE(EngineProfilerWidget)
    EditorWidget::Update(_deltaTime);

    const float elaspedTime =Timer::GetProgramElaspedTime();
    Ref<ProfileSession> activeProfileSession = ProfileSubsystem::GetActiveProfileSession();
    std::vector<Ref<ProfileSession>>& profileSessions = ProfileSubsystem::Get()->m_ProfileSessions;
    
    ImGui::Begin("Profiler");

    //ImGui::DragInt("Max FPS", &TimerSubsystem::GetMaxFPS(), 1, 0, 240);
    ImGui::SeparatorText("Engine Metrics");
    ImGui::Text("Program time: %fs", elaspedTime);
    ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTimeMs());
    ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());

    if (ImGui::CollapsingHeader("Thread Profiles"))
    {
        if (activeProfileSession)
        {
            const std::vector<Ref<Thread>>& threads = JobSubsystem::GetWorkerThreads();

            // Calc Load Difference
            float jobLoadMin = threads[0]->m_JobExecCount, jobLoadMax = 0;
            float execTimeMin = threads[0]->m_ThreadExecTime, execTimeMax = 0;

            for (const auto& thread : threads)
            {
                if (thread->m_JobExecCount > jobLoadMax) jobLoadMax = thread->m_JobExecCount;
                if (thread->m_JobExecCount < jobLoadMin) jobLoadMin = thread->m_JobExecCount;

                if (thread->m_ThreadExecTime > execTimeMax) execTimeMax = thread->m_ThreadExecTime;
                if (thread->m_ThreadExecTime < execTimeMin) execTimeMin = thread->m_ThreadExecTime;
            }

            float jobLoadDiff = 1.0f - jobLoadMin / jobLoadMax;
            float execTimeDiff = execTimeMin / execTimeMax;
            ImGui::Text("Load Diff: %f%", jobLoadDiff * 100.0f);
            ImGui::Text("Exec Diff: %f%", execTimeDiff * 100.0f);

            for (const auto& thread : threads)
            {
                ImGui::Text("ID: %d, Job Executed: %d, Thread Exec Time: %f, Thread Sleep Time: %f",
                            thread->m_ThreadIDInt, thread->m_JobExecCount, thread->m_ThreadExecTime,
                            thread->m_ThreadSleepTime);
            }
        }

    }
    
    ImGui::DragInt("Profile Average Count", &Profile::s_AverageDurationCount, 1, 3, 100);

    ImGui::SeparatorText("Profile Sessions");
    // if (ImGui::Button("Clear Profile Data")) ProfileSubsystem::Get()->m_ClearProfiles = true;
   

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
        std::unordered_map<std::string, Profile>& inlineProfiles = activeProfileSession->GetProfiles();
        std::unordered_map<std::string, JobProfile>& jobProfiles = activeProfileSession->GetJobProfiles();
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
            for (const auto& [name, profile] : inlineProfiles)
            {
                if (profile.m_DurationBuffer.ProfileResults.empty()) continue;
                    ImPlot::PlotLine(name.c_str(), &profile.m_DurationBuffer.ProfileResults[0].StartTime, &profile.m_DurationBuffer.ProfileResults[0].Duration,
                                     profile.m_DurationBuffer.ProfileResults.size(), 0, 0, 2 * sizeof(float));

                // Update the plot axis
                //profileMinDuration = std::min(profileMinDuration, profile.m_MinimumDuration);
                //profileMaxDuration = std::max(profileMaxDuration, profile.m_MaximumDuration);
            }
            ImPlot::EndPlot();
        }

        if (ImGui::CollapsingHeader("Active Profile Session"))
        {
            for (const auto& [name, profile] : inlineProfiles)
            {
                if (ImGui::TreeNode(name.c_str()))
                {
                    ImGui::Text("Minimum Duration: %fms", profile.m_MinimumDuration);
                    ImGui::Text("Maximum Duration: %fms", profile.m_MaximumDuration);
                    ImGui::Text("Average Duration: %fms", profile.m_AverageDuration);
                    ImGui::TreePop();
                }
            }
            
            // Options
                static ImGuiTableFlags flags =
                    ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
                    ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
                    | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                    ImGuiTableFlags_NoBordersInBody;
                   
                PushStyleCompact();
                ImGui::CheckboxFlags("ImGuiTableFlags_SortMulti", &flags, ImGuiTableFlags_SortMulti);    // HelpMarker("When sorting is enabled: hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).");
                ImGui::CheckboxFlags("ImGuiTableFlags_SortTristate", &flags, ImGuiTableFlags_SortTristate); //HelpMarker("When sorting is enabled: allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).");
                ImGui::CheckboxFlags("ImGuiTablefFlags_Resizable", &flags, ImGuiTableFlags_Resizable); //HelpMarker("Make the table columns resizable");
                ImGui::CheckboxFlags("ImGuiTableFlags_Reorderable", &flags, ImGuiTableFlags_Reorderable); //HelpMarker("Allow reordering columns in header row (need calling TableSetupColumn() + TableAutoHeaders() to display headers)");
                ImGui::CheckboxFlags("ImGuiTableFlags_Hideable", &flags, ImGuiTableFlags_Hideable); //HelpMarker("Allow hiding columns in context menu.");
                ImGui::CheckboxFlags("ImGuiTableFlags_Sortable", &flags, ImGuiTableFlags_Sortable); //HelpMarker("Allow sorting on one column when clicking on its header.");
                ImGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg); //HelpMarker("Draw horizontal background color on even rows.");
                ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuter", &flags, ImGuiTableFlags_BordersOuter); //HelpMarker("Draw horizontal borders between rows.");
                ImGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV); //HelpMarker("Draw horizontal borders between rows.");
                ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBody", &flags, ImGuiTableFlags_NoBordersInBody); //HelpMarker("Disable vertical borders in columns (borders will always appears in header row).");
                ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBodyUntilResize", &flags, ImGuiTableFlags_NoBordersInBodyUntilResize); //HelpMarker("Disable vertical borders in columns until hovered for resize (borders will always appears in header row).");
                ImGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX); //HelpMarker("Enable horizontal scrolling in table.");
                ImGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY); //HelpMarker("Enable vertical scrolling in table. Require 'OuterSize' parameter in BeginTable().");
                ImGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendX", &flags, ImGuiTableFlags_NoHostExtendX); //HelpMarker("Make outer width auto-fit to columns, overriding outer_size.x value.");
                ImGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendY", &flags, ImGuiTableFlags_NoHostExtendY); //HelpMarker("Make outer height stop at the bottom of the host window's visible area (when ScrollY is active).");
                ImGui::CheckboxFlags("ImGuiTableFlags_NoKeepColumnsVisible", &flags, ImGuiTableFlags_NoKeepColumnsVisible); //HelpMarker("Disable keeping column always minimally visible when ScrollX is off and table gets too small.");
                ImGui::CheckboxFlags("ImGuiTableFlags_PreciseWidths", &flags, ImGuiTableFlags_PreciseWidths); //HelpMarker("Make item width only be affected by the width of the label");
                ImGui::CheckboxFlags("ImGuiTableFlags_SizingFixedFit", &flags, ImGuiTableFlags_SizingFixedFit); //HelpMarker("Columns default to _WidthStretch with default width weights proportional to each columns contents.");
                PopStyleCompact();

                if (ImGui::BeginTable("table_sorting", 5, flags, ImVec2(0.0f, /*TEXT_BASE_HEIGHT * */ 15), 0.0f))
                {
                    // Declare columns
                    // We use the "user_id" parameter of TableSetupColumn() to specify a user id that will be stored in the sort specifications.
                    // This is so our sort function can identify a column given our own identifier. We could also identify them based on their index!
                    // Demonstrate using a mixture of flags among available sort-related flags:
                    // - ImGuiTableColumnFlags_DefaultSort
                    // - ImGuiTableColumnFlags_NoSort / ImGuiTableColumnFlags_NoSortAscending / ImGuiTableColumnFlags_NoSortDescending
                    // - ImGuiTableColumnFlags_PreferSortAscending / ImGuiTableColumnFlags_PreferSortDescending
                   // ImGui::TableSetupColumn("Thread", ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
                    ImGui::TableSetupColumn("Count", ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
                    ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 0.0f, 2);
                   // ImGui::TableSetupColumn("Avg", ImGuiTableColumnFlags_WidthFixed, 0.0f, 3);
                    ImGui::TableSetupColumn("Min", ImGuiTableColumnFlags_WidthFixed, 0.0f, 4);
                    ImGui::TableSetupColumn("Max", ImGuiTableColumnFlags_WidthFixed, 0.0f, 5);
                    ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
                    ImGui::TableHeadersRow();

                    /*// Sort our data if sort specs have been changed!
                    if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
                        if (sort_specs->SpecsDirty)
                        {
                            MyItem::SortWithSortSpecs(sort_specs, items.Data, items.Size);
                            sort_specs->SpecsDirty = false;
                        }*/

                    // Demonstrate using clipper for large vertical lists
                    if (jobProfiles.size() > 0)
                        for (const auto& profile : jobProfiles | std::views::values)
                        {
                            // Display a data item
                            ImGui::PushID(profile.GetName().c_str());
                            ImGui::TableNextRow();
                            //ImGui::TableNextColumn();
                            //ImGui::Text("%d", profile->m_LastThreadIndex);
                            ImGui::TableNextColumn();
                            ImGui::TextUnformatted(profile.GetName().c_str());
                            ImGui::TableNextColumn();
                            ImGui::Text("%d", profile.m_DurationBuffer.ProfileResults.size());
                            //ImGui::TableNextColumn();
                            //ImGui::Text("%f", profile->m_ExecTime);
                            ImGui::TableNextColumn();
                            ImGui::Text("%f", profile.m_AverageDuration);
                            ImGui::TableNextColumn();
                            ImGui::Text("%f", profile.m_MinimumDuration);
                            ImGui::TableNextColumn();
                            ImGui::Text("%f", profile.m_MaximumDuration);
                            ImGui::PopID();
                        }
                    ImGui::EndTable();
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

void PushStyleCompact()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                        ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.60f)));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                        ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.60f)));
}

void PopStyleCompact()
{
    ImGui::PopStyleVar(2);
}
