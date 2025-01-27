#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "Canvas.h"

#include "Denix/UI/UISubsystem.h"

void Canvas::BeginScene()
{
    UIWidget::BeginScene();

    m_IsActive = true;
    m_IsDisplayed = true;
    UISubsystem::GetInstance()->m_Widgets.push_back(GetRef<Canvas>());
}

void Canvas::EndScene()
{
    std::erase(UISubsystem::GetInstance()->m_Widgets, GetRef<Canvas>());
    
    UIWidget::EndScene();
}

void Canvas::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    UIWidget::Update(_deltaTime, _waitCounter);

    if (!m_IsActive) return;

    // Update Widgets
    for (const auto& widget : m_Buttons) widget->Update(_deltaTime, _waitCounter);
}
