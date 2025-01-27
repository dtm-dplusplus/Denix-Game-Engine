#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "UIWidget.h"
#include "Denix/Video/GL/Material.h"

Denix::UIWidget::UIWidget(): BaseObject({"UIWidget"}), m_ModelMatrix(1.0f)
{
    m_Material = MakeRef<Material>();
}

Denix::UIWidget::UIWidget(const ObjectInit& _objInit): BaseObject(_objInit), m_ModelMatrix(1.0f)
{
    m_Material = MakeRef<Material>();
}

void Denix::UIWidget::Disable()
{
    m_IsActive = false;
    m_IsDisplayed = false;
}

void Denix::UIWidget::Enable()
{
    m_IsActive = true;
    m_IsDisplayed = true;
}

void Denix::UIWidget::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    BaseObject::Update(_deltaTime, _waitCounter);
    
    CalculateModel(m_Transform, m_ModelMatrix);
}
