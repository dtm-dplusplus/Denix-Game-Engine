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
        
        void BeginScene() override;
        void EndScene() override;
        virtual void OnSelect();
        glm::vec3 defaultColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 selectedColor = glm::vec3(0.0f, 0.0f, 1.0f);
        
        Ref<AudioClip> m_SelectAudioClip;
    };
}
