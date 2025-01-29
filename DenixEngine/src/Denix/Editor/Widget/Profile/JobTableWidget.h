#pragma once
#include "imgui.h"
#include "Denix/Editor/EditorWidget.h"

// Make the UI compact because there are so many fields - From ImPlot Demo
static void PushStyleCompact();

static void PopStyleCompact();

namespace Denix
{
    class JobProfile;
    class ProfileSession;

    /**
     * Enum for job table column IDs.
     */
    enum JobTableColumnID
    {
        MyItemColumnID_Name, ///< Column ID for job name.
        MyItemColumnID_Count, ///< Column ID for job count.
        MyItemColumnID_Avg, ///< Column ID for average duration.
        MyItemColumnID_Min, ///< Column ID for minimum duration.
        MyItemColumnID_Max ///< Column ID for maximum duration.
    };

    /**
     * Struct for sorting job table.
     */
    struct SortJobTable
    {
        /**
         * Comparison operator for sorting job profiles.
         * @param a First job profile.
         * @param b Second job profile.
         * @param sort_specs Sorting specifications.
         * @return True if a should come before b, false otherwise.
         */
        bool operator()(const Ref<JobProfile>& a, const Ref<JobProfile>& b,
                        const ImGuiTableSortSpecs* sort_specs) const;
    };

    /**
     * JobTableWidget class for displaying and managing job profiles in a table.
     */
    class JobTableWidget final : public EditorWidget
    {
    public:
        /**
         * Constructor for JobTableWidget.
         */
        JobTableWidget();

        /**
         * Sorts the job profiles based on the specified criteria.
         * @param jobProfiles Vector of job profiles to sort.
         */
        void JobTableSort(std::vector<Ref<JobProfile>>& jobProfiles);

        /**
         * Updates the job table widget.
         * @param _deltaTime Time elapsed since the last update.
         */
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        /**
         * Reference to the active profile session.
         */
        Ref<ProfileSession> m_ProfileSession;
    };
}
