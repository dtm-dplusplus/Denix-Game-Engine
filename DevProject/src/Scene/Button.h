#pragma once
#include "Denix/Scene/Actor/Shapes.h"

namespace Denix
{
    class AudioClip;

    class Button: public Plane
    {
    public:
        Button();
        ~Button() override;

        virtual void OnSelect();
        inline static glm::vec3 defaultColor = glm::vec3(1.0f, 1.0f, 1.0f);
        inline static glm::vec3 selectedColor = glm::vec3(0.0f, 0.0f, 1.0f);
        
        Ref<AudioClip> m_SelectAudioClip;
    };
}
