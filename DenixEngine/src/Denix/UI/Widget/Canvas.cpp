#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "Canvas.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Input/InputSubsystem.h"

Denix::Canvas::Canvas()
{
    m_NavigateAudioClip = AssetSubsystem::GetAudioClip("Content\\Engine\\audio\\UI_Navigate.wav");
}

void Canvas::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    BaseObject::Update(_deltaTime, _waitCounter);

    if (!m_Buttons.empty() && !m_SelectedButton)
    {
        m_SelectedButton = m_Buttons[0];
        m_SelectedButton->GetRenderComponent()->GetMaterial()->m_BaseColor = m_SelectedButton->selectedColor;
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
                m_SelectedButton->GetRenderComponent()->GetMaterial()->m_BaseColor = m_SelectedButton->defaultColor;
                m_SelectedButton = m_Buttons[i + 1];
                m_SelectedButton->GetRenderComponent()->GetMaterial()->m_BaseColor = m_SelectedButton->selectedColor;
                AudioSubsystem::PlayAudioClipSingle(m_NavigateAudioClip.lock());
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
                m_SelectedButton->GetRenderComponent()->GetMaterial()->m_BaseColor = m_SelectedButton->defaultColor;
                m_SelectedButton = m_Buttons[i - 1];
                m_SelectedButton->GetRenderComponent()->GetMaterial()->m_BaseColor = m_SelectedButton->selectedColor;
                AudioSubsystem::PlayAudioClipSingle(m_NavigateAudioClip.lock());
                break;
            }
        }
    }
}