#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class PerformanceSettingsWidget: public EditorWidget
    {
    public:
        PerformanceSettingsWidget();

        void Update(float _deltaTime) override;
    };
}