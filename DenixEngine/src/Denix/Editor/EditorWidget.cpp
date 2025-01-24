#include "EditorWidget.h"


Denix::EditorWidget::EditorWidget(const ObjectInit& _objInit): Object(_objInit), m_IsOpen(true), m_IsFocus(false) 
{
}

void Denix::EditorWidget::Update(float _deltaTime)
{
    if (!m_IsOpen) MarkRubbish();
}
