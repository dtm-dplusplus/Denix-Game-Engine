#pragma once

#include "Denix/Core/BaseObject.h"
#include "Denix/Scene/Component/TransformPrimitive.h"


namespace Denix
{

    class UIWidget: public BaseObject
    {
    public:
        UIWidget(){}
        ~UIWidget() override {}

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
        Transform m_Transform;
        glm::mat4 m_Model;
        
        inline static bool s_WidgetLogging = true;
    };
}
