#pragma once
#include "Denix/UI/UIWidget.h"

namespace Denix
{
    class AudioClip;

    class Button: public UIWidget
    {
    public:
        Button();
        Button(const ObjectInit& _objInit);
        ~Button() override = default;
        
        virtual void OnSelect();
        virtual void OnDeselect() {}

        virtual void OnHoverBegin();
        virtual void OnHoverEnd();
        
        glm::vec3 m_DefaultColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 m_SelectedColor = glm::vec3(0.0f, 1.0f, 0.0f);

        WRef<UIWidget> m_CanvasParent;
        
        WRef<AudioClip> m_SelectAudioClip;
        WRef<AudioClip> m_HoverAudioClip;
    };
}
