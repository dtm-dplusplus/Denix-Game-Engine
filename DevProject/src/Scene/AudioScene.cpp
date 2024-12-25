#include "AudioScene.h"

#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Resource/Asset.h"

#include "imgui.h"
#include "Denix/Audio/AudioSource.h"
#include "Denix/Resource/ResourceSubsystem.h"

using namespace Denix;

void AudioScene::BeginScene()
{
    Scene::BeginScene();

    if (Ref<AudioClip> clip = ResourceSubsystem::GetAudioClip(FileSubsystem::GetContentRoot() + "Audio\\music.wav"))
    {
        Clip = clip;
        Source = MakeRef<AudioSource>("Audio Source_" + clip->GetName());
        Source->SetAudioClip(clip);
    }
}

void AudioScene::EndScene()
{
    Scene::EndScene();

    Source.reset();
    Clip.reset();
}

void AudioScene::DebugUI(float _deltaTime)
{
    Scene::DebugUI(_deltaTime);

    ImGui::Begin("Audio Scene");

    auto clip = Clip.lock();
    auto source = Source;
    
    if (clip && source)
    {
        switch (source->GetState())
        {
        case SourceState::Playing:
            {
                if (ImGui::Button("Pause"))
                    source->Pause();

                if (ImGui::Button("Stop"))
                    source->Stop();
            }
            break;

        case SourceState::Paused:
            {
                if (ImGui::Button("Resume"))
                    source->Play();

                if (ImGui::Button("Stop"))
                    source->Stop();
            }
            break;

        default:
            if (ImGui::Button("Play"))
                source->Play();
            break;
        }
    }
    ImGui::End();
}
