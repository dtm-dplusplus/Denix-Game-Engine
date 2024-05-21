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

        float Seperation;
        static float SeperationMax;

        float Height = 0.0f;

        int PipePairID = 0;

        Ref<Pipe> PipeTop;
        Ref<Pipe> PipeBottom;
    };
}


