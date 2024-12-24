#include "AudioScene.h"

#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Resource/Asset.h"

#include "imgui.h"
#include "Denix/Audio/AudioSource.h"

using namespace Denix;

void AudioScene::BeginScene()
{
    Scene::BeginScene();

    ClipAsset = MakeRef<Asset>(FileSubsystem::GetContentRoot() + "Audio\\music.wav");
    Clip = MakeRef<AudioClip>(ClipAsset);
    Source = MakeRef<AudioSource>("Audio Source_" + Clip->GetName());
    Source->SetAudioClip(Clip);
}

void AudioScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::Begin("Audio Scene");
    switch (Source->GetState())
    {
    case SourceState::Playing:
        {
            if (ImGui::Button("Pause"))
                Source->Pause();

            if (ImGui::Button("Stop"))
                Source->Stop();
        }
        break;

    case SourceState::Paused:
        {
            if (ImGui::Button("Resume"))
                Source->Play();

            if (ImGui::Button("Stop"))
                Source->Stop();
        }
        break;

    default:
        if (ImGui::Button("Play"))
            Source->Play();
        break;
    }
    ImGui::End();
}
