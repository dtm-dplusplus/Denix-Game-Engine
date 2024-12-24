#include "AudioScene.h"

#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Resource/Asset.h"

#include "imgui.h"

using namespace Denix;

void AudioScene::BeginScene()
{
    Scene::BeginScene();

    ClipAsset = MakeRef<Asset>(FileSubsystem::GetContentRoot() + "Audio\\music.wav");
    Clip = MakeRef<AudioClip>(ClipAsset);
}

void AudioScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::Begin("Audio Scene");
    switch (Clip->m_State)
    {
    case AudioClipState::Playing:
        {
            if (ImGui::Button("Pause"))
                Clip->Pause();

            if (ImGui::Button("Stop"))
                Clip->Stop();
        }
        break;

    case AudioClipState::Paused:
        {
            if (ImGui::Button("Resume"))
                Clip->Play();

            if (ImGui::Button("Stop"))
                Clip->Stop();
        }
        break;

    default:
        if (ImGui::Button("Play"))
            Clip->Play();
        break;
    }
    ImGui::End();
}
