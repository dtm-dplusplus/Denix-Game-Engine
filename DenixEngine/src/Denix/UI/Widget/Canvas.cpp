#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "Canvas.h"

#include "Denix/UI/UISubsystem.h"

using namespace Denix;

Canvas::Canvas(): UIWidget({"Canvas"})
{
    m_IsActive = true;
    m_IsDisplayed = true;
}

Canvas::Canvas(const ObjectInit& _objInit): UIWidget(_objInit)
{
    m_IsActive = true;
    m_IsDisplayed = true;
}

void Canvas::Enable()
{
    UIWidget::Enable();

    for (const auto& widget : m_Buttons) widget->Enable();
}

void Canvas::Disable()
{
    UIWidget::Disable();

    for (const auto& widget : m_Buttons) widget->Disable();
}

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

    m_Buttons.clear();
    m_SelectedButton.reset();
    
    UIWidget::EndScene();
}

void Canvas::Sort()
{
    // Sort the buttons by y position
    std::ranges::sort(m_Buttons, [](const Ref<Button>& a, const Ref<Button>& b)
    { return a->m_Transform.Position.y >
        b->m_Transform.Position.y; });
}

void Canvas::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    if (!m_IsActive) return;

    UIWidget::Update(_deltaTime, _waitCounter);

    // Update Widgets
    for (const auto& widget : m_Buttons) widget->Update(_deltaTime, _waitCounter);
}
