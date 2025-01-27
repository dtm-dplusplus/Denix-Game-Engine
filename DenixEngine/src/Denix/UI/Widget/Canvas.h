#pragma once
#include "Denix/UI/UIWidget.h"
#include "Button.h"

namespace Denix
{
    class Canvas: public UIWidget
    {
    public:
        Canvas();
        explicit Canvas(const ObjectInit& _objInit);
        ~Canvas() override = default;
        
        std::vector<Ref<Button>> m_Buttons;
        Ref<Button> m_SelectedButton;

        void Enable() override;
        void Disable() override;
        
        void BeginScene() override;
        void EndScene() override;
        void Sort();

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    };
}
