#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "Canvas.h"

#include "Denix/Input/InputSubsystem.h"

void Denix::Canvas::Update()
{
    if (!m_Buttons.empty() && !m_SelectedButton)
    {
        m_SelectedButton = m_Buttons[0];
        m_SelectedButton->GetRenderComponent()->GetMaterial()->GetBaseColor() = Button::selectedColor;
    }
    
    if (InputSubsystem::IsKeyUp(KeyCode::DEK_SPACE))
    {
        if (m_SelectedButton) m_SelectedButton->OnSelect();
    }
    if (InputSubsystem::IsKeyUp(KeyCode::DEK_DOWN))
    {
        for (size_t i = 0; i < m_Buttons.size(); ++i)
        {
            if (m_Buttons[i] == m_SelectedButton && (i + 1) < m_Buttons.size())
            {
                m_SelectedButton->GetRenderComponent()->GetMaterial()->GetBaseColor() = Button::defaultColor;
                m_SelectedButton = m_Buttons[i + 1];
                m_SelectedButton->GetRenderComponent()->GetMaterial()->GetBaseColor() = Button::selectedColor;
                break;
            }
        }
    }
    if (InputSubsystem::IsKeyUp(KeyCode::DEK_UP))
    {
        for (size_t i = 0; i < m_Buttons.size(); ++i)
        {
            if (m_Buttons[i] == m_SelectedButton && i > 0)
            {
                m_SelectedButton->GetRenderComponent()->GetMaterial()->GetBaseColor() = Button::defaultColor;
                m_SelectedButton = m_Buttons[i - 1];
                m_SelectedButton->GetRenderComponent()->GetMaterial()->GetBaseColor() = Button::selectedColor;
                break;
            }
        }
    }
}
