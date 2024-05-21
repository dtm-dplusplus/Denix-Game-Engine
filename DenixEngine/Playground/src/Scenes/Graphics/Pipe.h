#pragma once

#include "Denix/Scene/GameObject.h"

namespace Denix
{
    class Pipe : public GameObject
    {
    public:
        Pipe(const int _pipePairID, const bool _isTop, const ObjectInitializer& _object_init = ObjectInitializer::Get());
        ~Pipe() override = default;

        int PipePairID = 0;
        bool IsTop = false;
    };

    class PipePair : public GameObject
    {
    public:
        PipePair(const int _pipePairID, const ObjectInitializer& _object_init = ObjectInitializer::Get());
        ~PipePair() override = default;

        void InitPipePair();

        void Update(float _deltaTime) override;

        static float MoveSpeed;

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


