#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class JobTableWidget;

    /**
     * \class EngineProfilerWidget
     * \brief Widget for displaying engine profiling data.
     * \details The EngineProfilerWidget displays engine profiling data such as job profiling, frame times, and thread data.
     */
    class EngineProfilerWidget: public EditorWidget
    {
    public:
        EngineProfilerWidget();

        void Update(float _deltaTime) override;

        Ref<JobTableWidget> m_JobTableWidget;
    };
}
