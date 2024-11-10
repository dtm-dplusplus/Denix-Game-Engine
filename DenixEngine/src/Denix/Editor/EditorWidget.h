#pragma once
#include "Denix/Scene/Object.h"


namespace Denix
{
    class EditorWidget: public Object
    {
    public:
        EditorWidget(const ObjectInit& _objInit);
        ~EditorWidget() override = default;

        virtual void Begin() {}
        virtual void Update(float _deltaTime) {}
        virtual void End() {}

        static float m_DragSensitivity;
        static float m_DragSpeed;
    };
}
