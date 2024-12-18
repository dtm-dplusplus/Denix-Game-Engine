#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "JobTableWidget.h"

#include "imgui.h"
#include "Denix/Profile/ProfileSubsystem.h"

using namespace Denix;

ImGuiTableSortSpecs* MyItem::s_current_sort_specs = NULL;

JobTableWidget::JobTableWidget(): EditorWidget({"Job Table"})
{
    m_ProfileSession = ProfileSubsystem::GetActiveProfileSession();
}

void JobTableWidget::Update(float _deltaTime)
{
    EditorWidget::Update(_deltaTime);

    if (!m_ProfileSession) return;

    std::unordered_map<std::string, JobProfile>& jobProfiles = m_ProfileSession->GetJobProfiles();

    static ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
        ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
        | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
        ImGuiTableFlags_NoBordersInBody;

   if (ImGui::TreeNode("Table Flags"))
   {
        PushStyleCompact();
    ImGui::CheckboxFlags("ImGuiTableFlags_SortMulti", &flags, ImGuiTableFlags_SortMulti);
    // HelpMarker("When sorting is enabled: hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).");
    ImGui::CheckboxFlags("ImGuiTableFlags_SortTristate", &flags, ImGuiTableFlags_SortTristate);
    //HelpMarker("When sorting is enabled: allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).");
    ImGui::CheckboxFlags("ImGuiTablefFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
    //HelpMarker("Make the table columns resizable");
    ImGui::CheckboxFlags("ImGuiTableFlags_Reorderable", &flags, ImGuiTableFlags_Reorderable);
    //HelpMarker("Allow reordering columns in header row (need calling TableSetupColumn() + TableAutoHeaders() to display headers)");
    ImGui::CheckboxFlags("ImGuiTableFlags_Hideable", &flags, ImGuiTableFlags_Hideable);
    //HelpMarker("Allow hiding columns in context menu.");
    ImGui::CheckboxFlags("ImGuiTableFlags_Sortable", &flags, ImGuiTableFlags_Sortable);
    //HelpMarker("Allow sorting on one column when clicking on its header.");
    ImGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg);
    //HelpMarker("Draw horizontal background color on even rows.");
    ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuter", &flags, ImGuiTableFlags_BordersOuter);
    //HelpMarker("Draw horizontal borders between rows.");
    ImGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
    //HelpMarker("Draw horizontal borders between rows.");
    ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBody", &flags, ImGuiTableFlags_NoBordersInBody);
    //HelpMarker("Disable vertical borders in columns (borders will always appears in header row).");
    ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBodyUntilResize", &flags,
                         ImGuiTableFlags_NoBordersInBodyUntilResize);
    //HelpMarker("Disable vertical borders in columns until hovered for resize (borders will always appears in header row).");
    ImGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
    //HelpMarker("Enable horizontal scrolling in table.");
    ImGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);
    //HelpMarker("Enable vertical scrolling in table. Require 'OuterSize' parameter in BeginTable().");
    ImGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendX", &flags, ImGuiTableFlags_NoHostExtendX);
    //HelpMarker("Make outer width auto-fit to columns, overriding outer_size.x value.");
    ImGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendY", &flags, ImGuiTableFlags_NoHostExtendY);
    //HelpMarker("Make outer height stop at the bottom of the host window's visible area (when ScrollY is active).");
    ImGui::CheckboxFlags("ImGuiTableFlags_NoKeepColumnsVisible", &flags, ImGuiTableFlags_NoKeepColumnsVisible);
    //HelpMarker("Disable keeping column always minimally visible when ScrollX is off and table gets too small.");
    ImGui::CheckboxFlags("ImGuiTableFlags_PreciseWidths", &flags, ImGuiTableFlags_PreciseWidths);
    //HelpMarker("Make item width only be affected by the width of the label");
    ImGui::CheckboxFlags("ImGuiTableFlags_SizingFixedFit", &flags, ImGuiTableFlags_SizingFixedFit);
    //HelpMarker("Columns default to _WidthStretch with default width weights proportional to each columns contents.");
    PopStyleCompact();
       
    ImGui::TreePop();
   }

    if (!m_ProfileSession->IsProfiling())
    {
        // Declare columns
        // We use the "user_id" parameter of TableSetupColumn() to specify a user id that will be stored in the sort specifications.
        // This is so our sort function can identify a column given our own identifier. We could also identify them based on their index!
        // Demonstrate using a mixture of flags among available sort-related flags:
        // - ImGuiTableColumnFlags_DefaultSort
        // - ImGuiTableColumnFlags_NoSort / ImGuiTableColumnFlags_NoSortAscending / ImGuiTableColumnFlags_NoSortDescending
        // - ImGuiTableColumnFlags_PreferSortAscending / ImGuiTableColumnFlags_PreferSortDescending
        // ImGui::TableSetupColumn("Thread", ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
        if (ImGui::BeginTable((m_ProfileSession->GetName() + " Table").c_str(), 5, flags, ImVec2(0.0f, /*TEXT_BASE_HEIGHT * */ 15), 0.0f))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
            ImGui::TableSetupColumn("Count", ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
            ImGui::TableSetupColumn("Avg (ms)", ImGuiTableColumnFlags_WidthFixed, 0.0f, 2);
            // ImGui::TableSetupColumn("Avg", ImGuiTableColumnFlags_WidthFixed, 0.0f, 3);
            ImGui::TableSetupColumn("Min (ms)", ImGuiTableColumnFlags_WidthFixed, 0.0f, 4);
            ImGui::TableSetupColumn("Max (ms)", ImGuiTableColumnFlags_WidthFixed, 0.0f, 5);
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
            for (const auto& profile : jobProfiles | std::views::values)
            {
                // Display a data item
                if (!m_ProfileSession->IsProfiling())
                {
                    ImGui::PushID(profile.GetName().c_str());
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(profile.GetName().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", profile.m_DurationBuffer.ProfileResults.size());
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", profile.GetAverageDurationMs());
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", profile.GetMinDuration());
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", profile.GetMaxDuration());
                    ImGui::PopID();
                }
            }

            ImGui::EndTable();
        }
    }
    else
    {
        ImGui::NewLine();
        ImGui::Text("Finish Profiling to view results");
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
