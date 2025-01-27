#include "MainMenuScene.h"

#include "imgui.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/UI/UISubsystem.h"
#include "Game/UI/MainMenuCanvas.h"

using namespace Denix;

void MainMenuScene::BeginScene()
{
    Scene::BeginScene();

    m_MainMenuCanvas = MakeRef<MainMenuCanvas>();
    m_MainMenuCanvas->BeginScene();
}

void MainMenuScene::EndScene()
{
    Scene::EndScene();

    m_MainMenuCanvas->EndScene();
    m_MainMenuCanvas.reset();
}

void MainMenuScene::DebugUI(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::DebugUI(_deltaTime, _waitCounter);

    ImGui::Begin("Settings");
    ImGui::Text("Canvas: %s", m_MainMenuCanvas->GetName().c_str());
    for (const auto& button : m_MainMenuCanvas->m_Buttons)
    {
        //ImGui::Text("isactive: %d", m_MainMenuCanvas->m_IsActive);
       // ImGui::Text("isdisplayed: %d", m_MainMenuCanvas->m_IsDisplayed);
            ImGui::PushID(button->GetGUID());
            ImGui::Text("Button: %s", button->GetName().c_str());
            ImGui::DragFloat3("Position", &button->m_Transform.Position.x, 0.1f);
            ImGui::DragFloat3("Scale", &button->m_Transform.Scale.x, 0.1f);
            ImGui::DragFloat3("Selected Color", &button->m_SelectedColor.x, 0.1f);
            ImGui::DragFloat3("Default Color", &button->m_DefaultColor.x, 0.1f);
            ImGui::PopID();
    }
    
    ImGui::End();
}

void MainMenuScene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::Update(_deltaTime, _waitCounter);
    m_MainMenuCanvas->Update(_deltaTime, _waitCounter);
}
