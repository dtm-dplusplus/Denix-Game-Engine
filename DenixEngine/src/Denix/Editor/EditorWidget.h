#pragma once
#include "Denix/Scene/Object.h"


namespace Denix
{
    class EditorWidget: public Object
    {
    public:
        EditorWidget(const std::string& _objRef);
        ~EditorWidget() = default;

        virtual void Begin() {}
        virtual void Update() {}
        virtual void End() {}
    };
}
