#include "GEPScene.h"

#include "imgui.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Input/InputSubsystem.h"
#include "Game/Actor/BallActor.h"
#include "Game/UI/GameOverCanvas.h"
#include "Game/UI/MainMenuCanvas.h"

using namespace Denix;

void GEPScene::BeginPlay()
{
    Scene::BeginPlay();

    m_GameStart = false;
    m_GameOver = false;
    m_MenuCanvas = MakeRef<MainMenuCanvas>();
    m_MenuCanvas->BeginScene();
    
    m_GameOverCanvas = MakeRef<GameOverCanvas>();
    m_GameOverCanvas->BeginScene();

    m_MusicAudioSource = AudioSubsystem::CreateNewAudioSource();
    if (Ref<AudioClip> clip = AssetSubsystem::GetAudioClip("Content\\Audio\\music.wav"))
    {
        m_MusicClip = clip;
        m_MusicAudioSource->SetAudioClip(clip);
        m_MusicAudioSource->Play();
    }
}

void GEPScene::EndScene()
{
    Scene::EndScene();

    if (m_MenuCanvas) m_MenuCanvas->EndScene();
    m_MenuCanvas.reset();
    
    if (m_GameOverCanvas) m_GameOverCanvas->EndScene();
    m_GameOverCanvas.reset();
}

void GEPScene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::Update(_deltaTime, _waitCounter);

    if (!m_IsPlaying) return;
    
    if (m_GameOver)
    {
        DE_LOG(LogDevProject, Info, "Game Over");
        m_GameOverCanvas->Enable();
        m_GameOver = false;
        return;
    }

    if (!m_GameOverCanvas->m_IsActive && !m_MenuCanvas->m_IsActive)
    {
        if (Denix::InputSubsystem::IsKeyUp(Denix::KeyCode::DEK_SPACE))
        {
            Ref<BallActor> ball = SpawnActor<BallActor>(m_ActiveCamera->GetTransform().Position);
            ball->GetPhysicsComponent()->AddImpulse(m_ActiveCamera->GetTransformComponent()->GetForward() * ShootForce);
        }
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
    ImGui::End();
}
