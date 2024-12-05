#pragma once
#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    class FrameGraphWidget: public  EditorWidget
    {
    public:

        void Update(float _deltaTime) override;
    }; 
}
