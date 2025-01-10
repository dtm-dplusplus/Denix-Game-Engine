#include "CPGActor.h"

#include <PxRigidDynamic.h>

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Thread/JobSubsystem.h"
#include "Util/ActorGridSpawner.h"


Denix::CPGActor::CPGActor()
{
    // m_ClassName = "CPGActor";
    m_PhysicsComponent->SimulatePhysics() = true;
    m_PhysicsComponent->m_RotationEnabled = false;

    // Create a copy of the material
    m_RenderComponent->GetMaterial() = MakeRef<Material>(m_RenderComponent->GetMaterial());
}

void Denix::CPGActor::BeginPlay()
{
    Cube::BeginPlay();

    m_PhysicsComponent->GetMass() = Math::RandF(10.0f, 1000.0f);
}

void Denix::CPGActor::Update(float _deltaTime)
{
    Cube::Update(_deltaTime);

    // Reset the cube if it falls below the ground
    glm::vec3& pos = m_TransformComponent->GetPosition();
    if (pos.y < 0.0f)
    {
        pos.y = ActorGridSpawner::SpawnHeight;
        m_PhysicsComponent->GetVelocity() = glm::vec3(0.0f);
        m_PhysicsComponent->GetAngularVelocity() = glm::vec3(0.0f);
        if (physx::PxRigidDynamic* actor = m_PhysicsComponent->m_PxActor->is<physx::PxRigidDynamic>())
        {
            actor->setGlobalPose(physx::PxTransform(pos.x, pos.y, pos.z));
            actor->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
            actor->setAngularVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
        }
        RandomModel();
    }
}

void Denix::CPGActor::RandomModel() const
{
    static size_t modelStoreSize = AssetSubsystem::GetModelStore().size();
    static std::unordered_map<std::string, Ref<Model>> modelStore = AssetSubsystem::GetModelStore();
    static size_t textureStoreSize = AssetSubsystem::GetTextureStore().size();
    static std::unordered_map<std::string, Ref<Texture>> textureStore = AssetSubsystem::GetTextureStore();

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
