#pragma once
#include "imgui.h"
#include "Denix/Editor/EditorWidget.h"

// Make the UI compact because there are so many fields
static void PushStyleCompact();

static void PopStyleCompact();

namespace Denix
{
    class JobProfile;
    class ProfileSession;

    enum JobTableColumnID
    {
        MyItemColumnID_Name,
        MyItemColumnID_Count,
        MyItemColumnID_Avg,
        MyItemColumnID_Min,
        MyItemColumnID_Max
    };
    
    struct SortJobTable
    {
        bool operator()(const Ref<JobProfile>& a, const Ref<JobProfile>& b, const ImGuiTableSortSpecs* sort_specs) const;
    };
    
    class JobTableWidget final : public EditorWidget
    {
    public:
        JobTableWidget();
        void JobTableSort(std::vector<Ref<JobProfile>>& jobProfiles);
        void Update(float _deltaTime) override;
        // Comparison function for sorting job profiles
       
        Ref<ProfileSession> m_ProfileSession;
    };
}
