#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    /**
     * \brief PerformanceSettingsWidget class for managing performance settings in the editor.
     */
    class PerformanceSettingsWidget: public EditorWidget
    {
    public:
        /**
         * \brief Constructor for PerformanceSettingsWidget.
         */
        PerformanceSettingsWidget();

        /**
         * \brief Updates the performance settings widget.
         * \param _deltaTime Time elapsed since the last update.
         */
        void Update(float _deltaTime) override;

        /**
         * \brief Displays the performance settings widget.
         */
        static void Show();
    };
}