#pragma once
#include "Denix/Core/Object.h"


namespace Denix
{
    class EditorWidget: public Object
    {
    public:
        EditorWidget(const ObjectInit& _objInit);
        ~EditorWidget() override = default;

        virtual void Update(float _deltaTime);

        inline static float m_DragSensitivity = 10.0f;
        inline static float m_DragSpeed = 0.1f;

        bool m_IsOpen;
        bool m_IsFocus;
       // bool m_RequestClose;
    };
}
