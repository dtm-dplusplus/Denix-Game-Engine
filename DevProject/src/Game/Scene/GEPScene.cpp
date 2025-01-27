#include "GEPScene.h"

#include "imgui.h"
#include "Game/Actor/Character.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Input/InputSubsystem.h"
#include "Game/Actor/BallActor.h"
#include "Game/UI/GameOverCanvas.h"

void GEPScene::BeginScene()
{
    Scene::BeginScene();

    m_GameOver = false;
    m_GameOverCanvas = MakeRef<GameOverCanvas>();
    m_GameOverCanvas->BeginScene();
}

void GEPScene::EndScene()
{
    Scene::EndScene();

    m_GameOverCanvas->EndScene();
    m_GameOverCanvas.reset();
}

void GEPScene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::Update(_deltaTime, _waitCounter);

    if (m_GameOver)
    {
        DE_LOG(LogDevProject, Info, "Game Over");
        m_GameOverCanvas->Enable();
    }
    if (InputSubsystem::IsKeyUp(KeyCode::DEK_SPACE))
    {
        Ref<BallActor> ball = SpawnActor<BallActor>(m_ActiveCamera->GetTransform().Position);
        ball->GetPhysicsComponent()->AddImpulse(m_ActiveCamera->GetTransformComponent()->GetForward() * ShootForce);
    }
}

void GEPScene::DebugUI(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::DebugUI(_deltaTime, _waitCounter);

    ImGui::Begin("GEP Scene");
    ImGui::SeparatorText("Game");
    ImGui::Checkbox("Game Over", &m_GameOver);
    ImGui::DragFloat("Ball Mass", &BallActor::Mass, 0.1f, 0.1f, 30.0f);
    ImGui::DragFloat("Shoot Force", &ShootForce, 0.1f, 0.0f, 200.0f);
    ImGui::SeparatorText("Camera");
    auto camComp = m_ActiveCamera->GetComponent<CameraComponent>();
    ImGui::Checkbox("External Camera Control", &camComp->m_ExternalControl);
    ImGui::Checkbox("Rotation", &camComp->m_EnableRotation);

    if (m_Character)
    {
        static bool MatchCharacter = false;
        ImGui::Checkbox("Match Character", &MatchCharacter);
        static glm::vec3 offset = glm::vec3(0.0f, 1.0f, 0.0f);
        ImGui::DragFloat3("Offset", &offset[0], 0.1f);
        if (MatchCharacter) m_ActiveCamera->GetTransformComponent()->SetPosition(m_Character->GetTransformComponent()->GetPosition() + offset);
        
        ImGui::DragFloat("Move Speed", &m_Character->m_MoveSpeed, 0.1f, 0.0f, 200.0f);
        ImGui::DragFloat("Jump Force", &m_Character->m_JumpForce, 0.1f, 0.0f, 200.0f);
    }
    
    if (ImGui::Button("Spawn Character"))
    {
        m_Character = SpawnActor<Character>(glm::vec3(2.0f, 2.5f, 0.0f));
    }

    ImGui::Text("Canvas: %s", m_GameOverCanvas->GetName().c_str());
    for (const auto& button : m_GameOverCanvas->m_Buttons)
    {
        //ImGui::Text("isactive: %d", m_Canvas->m_IsActive);
        // ImGui::Text("isdisplayed: %d", m_Canvas->m_IsDisplayed);
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
