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

    m_MenuCanvas = MakeRef<MainMenuCanvas>();
    m_MenuCanvas->BeginScene();
    
    m_MusicAudioSource = AudioSubsystem::CreateNewAudioSource();
    if (Ref<AudioClip> clip = AssetSubsystem::GetAudioClip("Content\\Audio\\music.wav"))
    {
        m_MusicClip = clip;
        m_MusicAudioSource->SetAudioClip(clip);
        m_MusicAudioSource->Play();
    }

    // Spawn Cube Actors with random texture
    glm::vec3 SpawnLocation(0.0f, 0.0f, 0.0f);
    glm::vec3 SpawnRotation(0.0f, 0.0f, 0.0f);

    float CubeScale = 1.0f; // Size of each cube

    int CubeSize = 5; // Size of the big cube

    const std::unordered_map < std::string, Ref<Texture>>& textures = AssetSubsystem::GetTextureStore();
    size_t size = textures.size();
    
    for (int x = 0; x < CubeSize; ++x)
    {
        for (int y = 0; y < CubeSize; ++y)
        {
            for (int z = 0; z < CubeSize; ++z)
            {
                glm::vec3 CurrentLocation = SpawnLocation + glm::vec3(x * CubeScale, y * CubeScale, z * CubeScale);
                Ref<Actor> actor = SpawnActor<CubeActor>(CurrentLocation, SpawnRotation);
                Ref<Texture> texture;
                
                int t = 0, match = Math::Rand(0, size - 1);
                for(auto& [key, value] : textures)
                {
                    if(t == match)
                    {
                        texture = value;
                        break;
                    }
                    t++;
                }
                actor->GetRenderComponent()->GetMaterial()->SetBaseTexture(texture);
            }
        }
    }
}

void GEPScene::EndScene()
{
    Scene::EndScene();

    if (m_MenuCanvas) m_MenuCanvas->EndScene();
    m_MenuCanvas.reset();
    

    m_MusicAudioSource.reset();
}

void GEPScene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::Update(_deltaTime, _waitCounter);

    if (!IsPlaying()) return;
    
    // Only play the game if the menu is not active
    if (m_MenuCanvas->m_IsActive) return;

    // Spawn ball on space key up
    if (Denix::InputSubsystem::IsKeyUp(Denix::KeyCode::DEK_SPACE))
    {
        if (Ref<Actor> camera = GetActiveCamera())
        {
            Ref<TransformComponent> transform = camera->GetTransformComponent();
            glm::vec3 pos = transform->GetTransform().Position;
            glm::vec3 fwd = transform->GetForward();
            glm::vec3 impulse = fwd * ShootForce;

            if (Ref<BallActor> ball = SpawnActor<BallActor>(pos))
                ball->GetPhysicsComponent()->AddImpulse(impulse);
        }
    }
}

void GEPScene::ToolUpdate(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Scene::ToolUpdate(_deltaTime, _waitCounter);

    ImGui::Begin("GEP Scene");
    ImGui::SeparatorText("Game");
    ImGui::DragFloat("Ball Mass", &BallActor::Mass, 0.1f, 0.1f, 30.0f);
    ImGui::DragFloat("Shoot Force", &ShootForce, 0.1f, 0.0f, 200.0f);
    ImGui::End();
}
