#include "MainMenuCanvas.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"


void PlayButton::OnSelect()
{
    Button::OnSelect();

    SceneSubsystem::RequestOpenScene("Content\\Scene\\GEPScene.asset");
}

void QuitButton::OnSelect()
{
    Button::OnSelect();

    SceneSubsystem::RequestStop();
}

void MainMenuCanvas::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Canvas::Update(_deltaTime, _waitCounter);

    // Navigation Logic
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

void MainMenuCanvas::BeginScene()
{
    Canvas::BeginScene();

    m_NavigateAudioClip = AssetSubsystem::GetAudioClip("Content\\Engine\\audio\\UI_Navigate.wav");

    m_Buttons.push_back(MakeRef<PlayButton>());
    m_Buttons.back()->GetTransformComponent()->GetPosition().y = -1.0f;
    m_Buttons.back()->GetTransformComponent()->SetRotation(90.0f, 0.0f, 0.0f);
    m_Buttons.push_back(MakeRef<QuitButton>());
    m_Buttons.back()->GetTransformComponent()->GetPosition().y = -2.0f;
    m_Buttons.back()->GetTransformComponent()->SetRotation(90.0f, 0.0f, 0.0f);
    
    Sort();
}
