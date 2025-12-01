#include "GEPScene.h"

#include "imgui.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Input/InputSubsystem.h"
#include "Game/Actor/BallActor.h"
#include "Game/UI/MainMenuCanvas.h"

using namespace Denix;

void GEPScene::BeginPlay()
{
    Scene::BeginPlay();
    
    // Set clear color to emulate sky
    WindowSubsystem::GetWindow()->SetClearColor(glm::vec4(31.0f / 255.f, 169.f /255.f, 175.f/ 255.0f, 1.0f));

    // Disable Camera input until play button
    if (Ref<Actor> camera = GetActiveCamera())
    {
        if (Ref<CameraComponent> cameraComponent = camera->GetComponent<CameraComponent>())
        {
            cameraComponent->m_ExternalControl = true;
        }
        camera->GetTransformComponent()->SetPosition(2.0f, 3.0f, 30.0f);
        glm::vec3 pos = camera->GetTransformComponent()->GetTransform().Position;
        DE_LOG(Log, Info, "Camera Position: {} {} {}", pos.x, pos.y, pos.z);

    }

    m_MenuCanvas = MakeRef<MainMenuCanvas>();
    m_MenuCanvas->BeginScene();
    
    m_MusicAudioSource = AudioSubsystem::CreateNewAudioSource();
    if (Ref<AudioClip> clip = AssetSubsystem::GetAudioClip("Content\\Audio\\music.wav"); m_MusicAudioSource)
    {
        m_MusicAudioSource->m_Looping = true;
        m_MusicClip = clip;
        m_MusicAudioSource->SetAudioClip(clip);
        m_MusicAudioSource->Play();
    }

    // Spawn Cube Actors with random texture
    glm::vec3 SpawnLocation(0.0f, 0.0f, 0.0f);
    glm::vec3 SpawnRotation(0.0f, 0.0f, 0.0f);

    float CubeScale = 1.0f; // Size of each cube
    

    const std::unordered_map < std::string, Ref<Texture>>& textures = AssetSubsystem::GetTextureStore();
    size_t size = textures.size();
    
    for (int x = 0; x < CubeMultiplier; ++x)
    {
        for (int y = 0; y < CubeMultiplier; ++y)
        {
            for (int z = 0; z < CubeMultiplier; ++z)
            {
                glm::vec3 CurrentLocation = SpawnLocation + glm::vec3(x * CubeScale + 0.05f, (y * CubeScale + 0.05f) + 0.1f, z * CubeScale + 0.05f);
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

    WindowSubsystem::GetWindow()->SetClearColor(glm::vec4(0.f, 0.f, 0.f, 1.0f));
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
    ImGui::DragInt("Cube Multiplier", &CubeMultiplier, 1.0f, 1, 10);
    ImGui::DragFloat("Ball Mass", &BallActor::Mass, 0.1f, 0.1f, 30.0f);
    ImGui::DragFloat("Shoot Force", &ShootForce, 0.1f, 0.0f, 200.0f);
    ImGui::End();
}
