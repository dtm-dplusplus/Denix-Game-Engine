#pragma once
#include "Button.h"

using namespace Denix;

namespace Denix
{
    class AudioClip;
    class Canvas: public BaseObject
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

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        WRef<AudioClip> m_NavigateAudioClip;
    };
}
