#pragma once

#include "Denix/Core/BaseObject.h"
#include "Denix/Scene/Component/TransformPrimitive.h"


namespace Denix
{

}

namespace Denix
{
    class Model;
    class Material;
    
    class UIWidget: public BaseObject
    {
    public:
        UIWidget();
        UIWidget(const ObjectInit& _objInit);
        ~UIWidget() override  = default;

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        Ref<Material> m_Material;
        Ref<Model> m_Model;
        Transform m_Transform;
        glm::mat4 m_ModelMatrix;
        
        inline static bool s_WidgetLogging = true;
    };
}
