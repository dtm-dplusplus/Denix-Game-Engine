#include "JobSubsystemWidget.h"

#include "imgui.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"

ImGuiTableSortSpecs* MyItem::s_current_sort_specs = NULL;

namespace Denix
{
    void JobSubsystemWidget::Update(float _deltaTime)
    {
        EditorWidget::Update(_deltaTime);

        static std::vector<Ref<JobDeclaration>>& jobProfiles = JobSubsystem::Get()->JobProfileBuffer;
        static Ref<JobSubsystem> jobSubsystem = JobSubsystem::Get();
        std::vector<Ref<Thread>> threads = jobSubsystem->GetWorkerThreads();
        if (ImGui::CollapsingHeader("Job Subsystem"), ImGuiTreeNodeFlags_DefaultOpen)
        {
            ImGui::Text("System Threads Available: %d", jobSubsystem->GetActiveThreads());
            ImGui::Text("Worker Threads Available: %d", jobSubsystem->GetActiveThreads());
            ImGui::Text("Worker Threads Active: %d", jobSubsystem->GetActiveThreads());
            ImGui::Text("Jobs queued: %d", jobSubsystem->GetJobQueueSize());

            if (ImGui::InputInt("Active Threads", &jobSubsystem->GetActiveThreadsRef()))
            {
                JobSubsystem::UpdateActiveThreads();
            }


            if (ImGui::TreeNode("Profiling"))
            {
                if (ImGui::Checkbox("Thread Profiling", &Thread::s_ShouldProfile))
                    JobSubsystem::ToggleThreadProfiling();

                if (Thread::s_ShouldProfile)
                {
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

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Thread Settings"))
            {
                ImGui::DragInt("Wait For Counter Sleep Time", &Thread::s_WaitForCounterSleepTime);
                ImGui::DragInt("Wait For Job Sleep Time", &Thread::s_WaitForJobSleepTime);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Batch Settings"))
            {
                ImGui::DragInt("Batch Size", &SceneSubsystem::Get()->m_SceneBatchSize);
                ImGui::Checkbox("Batch Update", &SceneSubsystem::Get()->m_BatchUpdate);
                ImGui::Text("Batch Count: %d", SceneSubsystem::Get()->m_SceneBatchCount);
                ImGui::TreePop();
            }

                        {
                // Create item list
                static ImVector<MyItem> items;
                if (items.Size == 0)
                {
                    items.resize(50, MyItem());
                    for (int n = 0; n < items.Size; n++)
                    {
                        const int template_n = n % IM_ARRAYSIZE(template_items_names);
                        MyItem& item = items[n];
                        item.ID = n;
                        item.Name = template_items_names[template_n];
                        item.Quantity = (n * n - n) % 20; // Assign default quantities
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

                if (ImGui::BeginTable("table_sorting", 4, flags, ImVec2(0.0f, /*TEXT_BASE_HEIGHT * */ 15), 0.0f))
                {
                    // Declare columns
                    // We use the "user_id" parameter of TableSetupColumn() to specify a user id that will be stored in the sort specifications.
                    // This is so our sort function can identify a column given our own identifier. We could also identify them based on their index!
                    // Demonstrate using a mixture of flags among available sort-related flags:
                    // - ImGuiTableColumnFlags_DefaultSort
                    // - ImGuiTableColumnFlags_NoSort / ImGuiTableColumnFlags_NoSortAscending / ImGuiTableColumnFlags_NoSortDescending
                    // - ImGuiTableColumnFlags_PreferSortAscending / ImGuiTableColumnFlags_PreferSortDescending
                    ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed,
                                            0.0f, MyItemColumnID_ID);
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Name);
                    ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed,
                                            0.0f, MyItemColumnID_Action);
                    ImGui::TableSetupColumn(
                        "Quantity", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch,
                        0.0f, MyItemColumnID_Quantity);
                    ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
                    ImGui::TableHeadersRow();

                    // Sort our data if sort specs have been changed!
                    if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
                        if (sort_specs->SpecsDirty)
                        {
                            MyItem::SortWithSortSpecs(sort_specs, items.Data, items.Size);
                            sort_specs->SpecsDirty = false;
                        }

                    // Demonstrate using clipper for large vertical lists
                    ImGuiListClipper clipper;
                    clipper.Begin(items.Size);
                    while (clipper.Step())
                        for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
                        {
                            // Display a data item
                            MyItem* item = &items[row_n];
                            ImGui::PushID(item->ID);
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("%04d", item->ID);
                            ImGui::TableNextColumn();
                            ImGui::TextUnformatted(item->Name);
                            ImGui::TableNextColumn();
                            ImGui::SmallButton("None");
                            ImGui::TableNextColumn();
                            ImGui::Text("%d", item->Quantity);
                            ImGui::PopID();
                        }
                    ImGui::EndTable();
                }
            }
        }
    }
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
