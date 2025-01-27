#include "Buttons.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"

using namespace Denix;

PlayButton::PlayButton(): Button({"PlayButton"})
{
    m_Material->SetBaseTexture(AssetSubsystem::GetTexture("Content\\Textures\\UI_Play.png"));
}

void PlayButton::OnSelect()
{
    Button::OnSelect();

    SceneSubsystem::RequestOpenScene("Content\\Scene\\GEPScene.asset");
}

RestartButton::RestartButton(): Button({"RestartButton"})
{
    m_Material->SetBaseTexture(AssetSubsystem::GetTexture("Content\\Textures\\UI_Restart.png"));
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

void RestartButton::OnSelect()
{
    Button::OnSelect();

    SceneSubsystem::RequestOpenScene("Content\\Scene\\GEPScene.asset");
}

MainMenuButton::MainMenuButton(): Button({"MainMenuButton"})
{
    m_Material->SetBaseTexture(AssetSubsystem::GetTexture("Content\\Textures\\UI_Menu.png"));
}

void MainMenuButton::OnSelect()
{
    Button::OnSelect();

    SceneSubsystem::RequestOpenScene("Content\\Scene\\MainMenuScene.asset");
}

