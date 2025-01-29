#pragma once

#include "Denix/Core/BaseObject.h"
#include "Denix/Scene/Component/TransformPrimitive.h"

namespace Denix
{
    class Model;
    class Material;

    class UIWidget : public BaseObject
    {
    public:
        UIWidget();
        UIWidget(const ObjectInit& _objInit);
        ~UIWidget() override = default;

        virtual void Disable();
        virtual void Enable();
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        Ref<Material> m_Material;
        WRef<Model> m_Model;
        Transform m_Transform;
        glm::mat4 m_ModelMatrix;

        bool m_IsDisplayed;
        bool m_IsActive;
    };
}
