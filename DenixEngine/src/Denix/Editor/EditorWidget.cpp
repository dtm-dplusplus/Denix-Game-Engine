#include "EditorWidget.h"


Denix::EditorWidget::EditorWidget(const ObjectInit& _objInit): Object(_objInit), m_IsOpen(true), m_IsFocus(false) 
{
}

void Denix::EditorWidget::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    if (!m_IsOpen) MarkRubbish();
}
