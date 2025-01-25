#pragma once

#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class InputDebuggerWidget: public EditorWidget
    {
    public:
        InputDebuggerWidget();

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    };
}