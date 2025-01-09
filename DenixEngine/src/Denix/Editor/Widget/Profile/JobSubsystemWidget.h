#pragma once
#include "Denix/Editor/EditorWidget.h"



namespace Denix
{
    class JobSubsystemWidget: public EditorWidget
    {
    public:
        JobSubsystemWidget(const ObjectInit& _objInit)
            : EditorWidget(_objInit)
        {
        }
        void Update(float _deltaTime) override;
    };
}
