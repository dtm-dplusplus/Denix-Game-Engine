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
        m_PhysicsComponent->IsColliderVisible() = true;
        m_PhysicsComponent->CollisionDimensionOverride() = true;
        m_PhysicsComponent->SetCollider(col);
    }

    PipePair::PipePair(const int _pipePairID, const ObjectInitializer& _object_init): GameObject(_object_init)
    {
        PipePairID = _pipePairID;

        PipeTop = MakeRef<Pipe>(_pipePairID, true, _object_init);

        PipeBottom = MakeRef<Pipe>(_pipePairID, false, _object_init);
        PipeBottom->IsTop = false;

        // Rotate Top Pipe
        PipeTop->GetTransformComponent()->GetRotation().x = 180.0f;

        // Set Pipe Pair Seperation
        Seperation = 2.0f;
        PipeTop->GetTransformComponent()->GetPosition().y = Seperation / 2.0f;
        PipeBottom->GetTransformComponent()->GetPosition().y = -Seperation / 2.0f;

        // Set Pipe Pair Height
        Height = 0.0f;
        PipeTop->GetTransformComponent()->GetPosition().y += Height;
        PipeBottom->GetTransformComponent()->GetPosition().y += Height;
    }
}