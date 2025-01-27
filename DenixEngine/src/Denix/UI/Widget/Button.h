#pragma once
#include "Denix/UI/UIWidget.h"

namespace Denix
{
    class AudioClip;

    class Button: public UIWidget
    {
    public:
        Button();
        ~Button() override = default;
        
        void BeginScene() override;
        
        void EndScene() override;
        
        virtual void OnSelect();
        
        glm::vec3 m_DefaultColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 m_SelectedColor = glm::vec3(0.0f, 0.0f, 1.0f);
        
        Ref<AudioClip> m_SelectAudioClip;
    };
}
