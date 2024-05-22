#pragma once

#include "Denix/Scene/GameObject.h"

namespace Denix
{
    class Pipe : public GameObject
    {
    public:
        Pipe(const int _pipePairID, const bool _isTop, const ObjectInitializer& _object_init = ObjectInitializer::Get()) : GameObject(_object_init)
        {
            PipePairID = _pipePairID;
            IsTop = _isTop;


            Ref<CubeCollider> col = MakeRef<CubeCollider>();
            col->GetDimensions() = { 0.3f, 8.0f, 0.3f };
            col->GetOffset().y = IsTop ? 8.0f : -8.0f;
            m_PhysicsComponent->IsColliderVisible() = false;
            m_PhysicsComponent->CollisionDimensionOverride() = true;
            m_PhysicsComponent->SetCollider(col);

            if (IsTop)
            {
                m_TransformComponent->GetRotation().x = 180.0f;
            }
        }

        ~Pipe() override = default;

        int PipePairID = 0;
        bool IsTop = false;
    };

    class PipePair : public GameObject
    {
    public:
        PipePair(const int _pipePairID, const ObjectInitializer& _object_init = ObjectInitializer::Get()): GameObject(_object_init)
        {
            PipePairID = _pipePairID;

            PipeTop = MakeRef<Pipe>(_pipePairID, true, ObjectInitializer("PipeTop Pair " + std::to_string(PipePairID)));
            PipeBottom = MakeRef<Pipe>(_pipePairID, false, ObjectInitializer("PipeBottom Pair " + std::to_string(PipePairID)));
        }
        ~PipePair() override = default;

        void InitPipePair();

        void GameUpdate(float _deltaTime) override;

        static float MoveSpeed;
        static bool CanMove;

        float Seperation;
        static float SeperationMax;
        static float SeperationMin;

        /* Distance in -x axis when this object should be destroyed - Optimizes framerate */
        static float DestructionDistance;

        int PipePairID = 0;

        Ref<Pipe> PipeTop;
        Ref<Pipe> PipeBottom;
    };
}


