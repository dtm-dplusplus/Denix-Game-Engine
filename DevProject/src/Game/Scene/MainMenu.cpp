#include "MainMenu.h"

#include "Denix/Scene/SceneSubsystem.h"
#include "Game/UI/MainMenuCanvas.h"

void MainMenu::BeginScene()
{
    Scene::BeginScene();

    m_Canvas = MakeRef<MainMenuCanvas>();
    m_Canvas->BeginScene();
}

void MainMenu::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::Update(_deltaTime, _waitCounter);
    m_Canvas->Update(_deltaTime, _waitCounter);
}
