#pragma once
#include "Button.h"
#include "Denix/UI/UIWidget.h"

using namespace Denix;

namespace Denix
{
    class AudioClip;
    class Canvas: public UIWidget
    {
    public:
        Canvas() = default;
        ~Canvas() override = default;
        
        std::vector<Ref<Button>> m_Buttons;
        Ref<Button> m_SelectedButton;

        void BeginScene() override;
        void EndScene() override;
        void Sort()
        {
            // Sort the buttons by y position
            std::ranges::sort(m_Buttons, [](const Ref<Button>& a, const Ref<Button>& b)
                { return a->m_Transform.Position.y >
                b->m_Transform.Position.y; });
        }

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

        bool m_IsDisplayed = false;
        bool m_IsActive = false;
    };
}
