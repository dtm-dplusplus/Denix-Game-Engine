#include "GACPActor.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Core/Thread/JobSubsystem.h"
#include "GACPSpawner.h"
#include <PxRigidDynamic.h>

Denix::GACPActor::GACPActor()
{
    // m_ClassName = "CPGActor";
    m_PhysicsComponent->SimulatePhysics() = true;
    m_PhysicsComponent->m_RotationEnabled = false;
    m_PhysicsComponent->GetMass() = Math::RandF(10.0f, 10000.0f);
    
    // Create a copy of the material
    m_RenderComponent->GetMaterial() = MakeRef<Material>(m_RenderComponent->GetMaterial());
    RandomModel();
}

void Denix::GACPActor::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
    Cube::Update(_deltaTime, _waitCounter);

    // Reset the cube if it falls below the ground
    glm::vec3& pos = m_TransformComponent->GetPosition();
    if (pos.y < 0.0f)
    {
        pos.y = GACPSpawner::SpawnHeight;
        m_PhysicsComponent->GetVelocity() = glm::vec3(0.0f);
        m_PhysicsComponent->GetAngularVelocity() = glm::vec3(0.0f);
        if (physx::PxRigidDynamic* actor = m_PhysicsComponent->m_PxActor->is<physx::PxRigidDynamic>())
        {
            actor->setGlobalPose(physx::PxTransform(pos.x, pos.y, pos.z));
            actor->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
            actor->setAngularVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
        }
    }
}

void Denix::GACPActor::RandomModel() const
{
    static size_t modelStoreSize = AssetSubsystem::GetModelStore().size();
    static std::unordered_map<std::string, Ref<Model>>& modelStore = AssetSubsystem::GetModelStore();
    static size_t textureStoreSize = AssetSubsystem::GetTextureStore().size();
    static std::unordered_map<std::string, Ref<Texture>>& textureStore = AssetSubsystem::GetTextureStore();

    size_t index = Math::Rand(0, modelStoreSize - 1);
    size_t i = 0;
    for (const auto& model: modelStore | std::views::keys)
    {
        if (i++ == index)
        {
            m_MeshComponent->SetModel(modelStore[model]);
            break;
        }
    }

    index = Math::Rand(0, textureStoreSize - 1);
    i = 0;
    for (const auto& texture: textureStore | std::views::keys)
    {
        if (i++ == index)
        {
            m_RenderComponent->GetMaterial()->GetBaseTexture()  = textureStore[texture];
            break;
        }
    }
}