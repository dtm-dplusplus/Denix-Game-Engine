#include "MainMenuCanvas.h"
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

void MainMenuCanvas::BeginScene()
{
    Canvas::BeginScene();

    Ref<Scene> scene = SceneSubsystem::GetActiveScene();
    m_Buttons.push_back(scene->SpawnActor<PlayButton>());
    m_Buttons.back()->GetTransformComponent()->GetPosition().y = -1.0f;
    m_Buttons.back()->GetTransformComponent()->SetRotation(90.0f, 0.0f, 0.0f);
    m_Buttons.push_back(scene->SpawnActor<QuitButton>());
    m_Buttons.back()->GetTransformComponent()->GetPosition().y = -2.0f;
    m_Buttons.back()->GetTransformComponent()->SetRotation(90.0f, 0.0f, 0.0f);
    Sort();
}
