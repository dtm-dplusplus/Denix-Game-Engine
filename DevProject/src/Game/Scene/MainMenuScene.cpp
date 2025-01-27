#include "MainMenuScene.h"

#include "imgui.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/UI/UISubsystem.h"
#include "Game/UI/MainMenuCanvas.h"

void MainMenuScene::BeginScene()
{
    Scene::BeginScene();

    m_Canvas = MakeRef<MainMenuCanvas>();
    m_Canvas->BeginScene();
    for (auto& button : m_Canvas->m_Buttons)
    {
        Transform transform = button->m_Transform;
        //SpawnActor(button, transform.Position, transform.Rotation, transform.Scale);
    }
}

void MainMenuScene::DebugUI(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::DebugUI(_deltaTime, _waitCounter);

    ImGui::Begin("Settings");
    for (const auto& canvas : UISubsystem::GetInstance()->m_Widgets)
    {
        ImGui::Text("Canvas: %s", canvas->GetName().c_str());
        ImGui::Text("isactive: %d", canvas->m_IsActive);
        ImGui::Text("isdisplayed: %d", canvas->m_IsDisplayed);
        for (const auto& button : canvas->m_Buttons)
        {
            ImGui::Text("Button: %s", button->GetName().c_str());
            ImGui::DragFloat3("Position", &button->m_Transform.Position.x, 0.1f);
            ImGui::DragFloat3("Scale", &button->m_Transform.Scale.x, 0.1f);
            ImGui::DragFloat3("Selected Color", &button->m_SelectedColor.x, 0.1f);
            ImGui::DragFloat3("Default Color", &button->m_DefaultColor.x, 0.1f);
        }
    }
    
    ImGui::End();
}

void MainMenuScene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::Update(_deltaTime, _waitCounter);
    m_Canvas->Update(_deltaTime, _waitCounter);
}
