#include "CPGCube.h"

#include "Util/ActorGridSpawner.h"

CPGCube::CPGCube()
{
    m_ClassName = "CPGCube";
    m_PhysicsComponent->SimulatePhysics() = true;
}

void CPGCube::BeginPlay()
{
    Cube::BeginPlay();

    m_PhysicsComponent->GetMass() = Math::RandF(10.0f, 100.0f);
}

void CPGCube::Update(float _deltaTime)
{
    Cube::Update(_deltaTime);

    // Reset the cube if it falls below the ground
    glm::vec3& pos = m_TransformComponent->GetPosition();
    if (pos.y < -10.0f)
    {
        pos.y = ActorGridSpawner::SpawnHeight;
        m_PhysicsComponent->GetVelocity() = glm::vec3(0.0f);
        m_PhysicsComponent->GetAngularVelocity() = glm::vec3(0.0f);
    }
}