#include "Buttons.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Game/Scene/GEPScene.h"

using namespace Denix;

PlayButton::PlayButton(): Button({"PlayButton"})
{
    m_Material->SetBaseTexture(AssetSubsystem::GetTexture("Content\\Textures\\UI_Play.png"));
}

void PlayButton::OnSelect()
{
    Button::OnSelect();
    
    if (auto canvas = m_CanvasParent.lock())
    {
        // Hide the canvas
        canvas->Disable();
        
        // Enable Camera input
        if (auto scene = SceneSubsystem::GetActiveScene())
        {
            if (auto camera = scene->GetActiveCameraComponent())
            {
                camera->m_ExternalControl = false;
            }
        }
    }
}

QuitButton::QuitButton(): Button({"QuitButton"})
{
    m_Material->SetBaseTexture(AssetSubsystem::GetTexture("Content\\Textures\\UI_Quit.png"));
}

void QuitButton::OnSelect()
{
    Button::OnSelect();

    SceneSubsystem::RequestStop();
}