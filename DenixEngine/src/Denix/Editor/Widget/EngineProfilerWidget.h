#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class EngineProfilerWidget: public EditorWidget
    {
    public:
        EngineProfilerWidget();

        void Update(float _deltaTime) override;
    };
}