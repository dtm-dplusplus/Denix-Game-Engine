#include "AudioScene.h"

#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Asset/Asset.h"

#include "imgui.h"
#include "Denix/Audio/AudioSource.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Core/FileSubsystem.h"

using namespace Denix;

void AudioScene::BeginScene()
{
    Scene::BeginScene();

    std::string relPath =  "Content\\Audio\\music.wav";
    relPath = FileSubsystem::FormatPath(relPath);
    //DE_LOG(LogScene, Info, "rel {}", FileSubsystem::FormatRelativePath(FileSubsystem::GetContentRoot()));
    DE_LOG(LogScene, Info, "abs {}", FileSubsystem::FormatPath(relPath));
    DE_LOG(LogScene, Info, "rel {}", FileSubsystem::FormatRelativePath(relPath))
    if (Ref<AudioClip> clip = AssetSubsystem::GetAudioClip(FileSubsystem::GetContentRoot() + "Audio\\music.wav"))
    {
        Clip = clip;
        Source = MakeRef<AudioSource>();
        Source->SetAudioClip(clip);
    }
    
    FileSubsystem::CopyFileDE(FileSubsystem::GetContentRoot() + "Audio\\file.txt", FileSubsystem::GetContentRoot());
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
