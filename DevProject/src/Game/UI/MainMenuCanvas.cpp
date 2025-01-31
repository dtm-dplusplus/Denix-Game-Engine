#include "MainMenuCanvas.h"

#include "Buttons.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"

using namespace Denix;

void MainMenuCanvas::BeginScene()
{
    Canvas::BeginScene();
}


MainMenuCanvas::MainMenuCanvas()
{
    m_Buttons.push_back(MakeRef<PlayButton>());
    m_Buttons.back()->m_Transform.Position.y = 0.3f;
    
    m_Buttons.push_back(MakeRef<QuitButton>());
    m_Buttons.back()->m_Transform.Position.y = -0.3f;
    Sort();
}

void MainMenuCanvas::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    if (!m_IsActive) return;
    Canvas::Update(_deltaTime, _waitCounter);

    // Navigation Logic
    if (!m_Buttons.empty() && !m_SelectedButton)
    {
        m_SelectedButton = m_Buttons[0];
        m_SelectedButton->OnHoverBegin();
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
                m_SelectedButton->OnHoverEnd();
                m_SelectedButton = m_Buttons[i + 1];
                m_SelectedButton->OnHoverBegin();
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
                m_SelectedButton->OnHoverEnd();
                m_SelectedButton = m_Buttons[i - 1];
                m_SelectedButton->OnHoverBegin();
                break;
            }
        }
    }
}

