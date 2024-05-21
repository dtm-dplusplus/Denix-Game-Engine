#include "Pipe.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Physics/Collider.h"

namespace Denix
{
    Pipe::Pipe(const int _pipePairID, const bool _isTop, const ObjectInitializer& _object_init): GameObject(_object_init)
    {
        PipePairID = _pipePairID;
        IsTop = _isTop;

        m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Pipe"));

        Ref<CubeCollider> col = MakeRef<CubeCollider>();
        col->GetDimensions() = { 0.3f, 8.0f, 0.3f };
        col->GetOffset().y =  IsTop? 8.0f : -8.0f;
        m_PhysicsComponent->IsColliderVisible() = false;
        m_PhysicsComponent->CollisionDimensionOverride() = true;
        m_PhysicsComponent->SetCollider(col);

        if (IsTop)
        {
            m_TransformComponent->GetRotation().x = 180.0f;
        }
    }

    float PipePair::MoveSpeed{0.5f};
    float PipePair::SeperationMax{3.0f};
    float PipePair::SeperationMin{0.5f};
    float PipePair::DestructionDistance{100.0f};




    PipePair::PipePair(const int _pipePairID, const ObjectInitializer& _object_init): GameObject(_object_init)
    {
        PipePairID = _pipePairID;

        PipeTop = MakeRef<Pipe>(_pipePairID, true, ObjectInitializer("PipeTop Pair " + std::to_string(PipePairID)));
        PipeBottom = MakeRef<Pipe>(_pipePairID, false, ObjectInitializer("PipeBottom Pair " + std::to_string(PipePairID)));
    }

    void PipePair::InitPipePair()
    {
        // Generate Random Seperation
        Seperation = 2.0f;

        // Set Pipe Positions
        const glm::vec3& pairPosition = m_TransformComponent->GetPosition();
        PipeTop->GetTransformComponent()->SetPosition(
            { pairPosition.x, pairPosition.y + Seperation / 2.0f, pairPosition.z });

        PipeBottom->GetTransformComponent()->SetPosition(
            { pairPosition.x, pairPosition.y + -Seperation / 2.0f, pairPosition.z });
    }
    void PipePair::Update(float _deltaTime)
    {
        glm::vec3& position = m_TransformComponent->GetPosition();
        position.x -= MoveSpeed * _deltaTime;
        PipeTop->GetTransformComponent()->SetPosition(position);
        PipeBottom->GetTransformComponent()->SetPosition(position);

        if (m_TransformComponent->GetPosition().x <= -DestructionDistance)
        {
            PipeTop->Destroy();
            PipeBottom->Destroy();
            Destroy();
        }
    }
}