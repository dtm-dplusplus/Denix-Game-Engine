#pragma once
#include "Denix/Core/BaseObject.h"

namespace Denix
{
    class UIWidget: public BaseObject
    {
    public:
        UIWidget(){}
        ~UIWidget() override {}

        inline static bool s_WidgetLogging = true;
    };
}
