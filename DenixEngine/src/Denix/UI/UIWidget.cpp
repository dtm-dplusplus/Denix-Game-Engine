#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "UIWidget.h"

void Denix::UIWidget::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    BaseObject::Update(_deltaTime, _waitCounter);

    CalculateModel(m_Transform, m_Model);
}
