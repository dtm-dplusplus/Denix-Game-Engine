#pragma once
#include "Button.h"

namespace Denix
{
    class AudioClip;
}

namespace Denix
{
    class Canvas
    {
    public:
        Canvas();
        std::vector<Ref<Button>> m_Buttons;
        Ref<Button> m_SelectedButton;
        
        void Sort()
        {
            // Sort the buttons by y position
            std::ranges::sort(m_Buttons, [](const Ref<Button>& a, const Ref<Button>& b)
                { return a->GetTransformComponent()->GetTransform().Position.y >
                b->GetTransformComponent()->GetTransform().Position.y; });
        }

        void Update();

        WRef<AudioClip> m_NavigateAudioClip;
    };
}
