#pragma once
#include "Denix/Scene/Object.h"


namespace Denix
{
    class EditorWidget: public Object
    {
    public:
        EditorWidget(const std::string& _objRef);
        ~EditorWidget() override = default;

        virtual void Begin() {}
        virtual void Update(float _deltaTime) {}
        virtual void End() {}
    };
}
