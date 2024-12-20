#include "CPGCube.h"

#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"
#include "Util/ActorGridSpawner.h"

CPGCube::CPGCube()
{
    m_ClassName = "CPGCube";
    m_PhysicsComponent->SimulatePhysics() = true;
    m_PhysicsComponent->m_RotationEnabled = false;

    // Create a copy of the material
    m_RenderComponent->GetMaterial() = MakeRef<Material>(m_RenderComponent->GetMaterial());
}

void CPGCube::BeginPlay()
{
    Cube::BeginPlay();

    m_PhysicsComponent->GetMass() = Math::RandF(10.0f, 1000.0f);
}

void CPGCube::Update(float _deltaTime)
{
    Cube::Update(_deltaTime);

    // Reset the cube if it falls below the ground
    glm::vec3& pos = m_TransformComponent->GetPosition();
    if (pos.y < 0.0f)
    {
        pos.y = ActorGridSpawner::SpawnHeight;
        m_PhysicsComponent->GetVelocity() = glm::vec3(0.0f);
        m_PhysicsComponent->GetAngularVelocity() = glm::vec3(0.0f);

        RandomModel();
    }
}

void CPGCube::RandomModel()
{
    static size_t modelStoreSize = ResourceSubsystem::GetModelStore().size();
    static std::unordered_map<std::string, Ref<Model>> modelStore = ResourceSubsystem::GetModelStore();
    static size_t textureStoreSize = ResourceSubsystem::GetTextureStore().size();
    static std::unordered_map<std::string, Ref<Texture>> textureStore = ResourceSubsystem::GetTextureStore();

    size_t index = Math::Rand(0, modelStoreSize - 1);
    size_t i = 0;
    for (auto model: modelStore | std::views::keys)
    {
        if (i++ == index)
        {
            m_MeshComponent->SetModel(modelStore[model]);
            break;
        }
    }

    index = Math::Rand(0, textureStoreSize - 1);
    i = 0;
    for (auto texture: textureStore | std::views::keys)
    {
        if (i++ == index)
        {
            m_RenderComponent->GetMaterial()->GetBaseTexture()  = textureStore[texture];
            break;
        }
    }
}
